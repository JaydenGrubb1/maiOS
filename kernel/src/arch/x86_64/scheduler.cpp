/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-18
 * @brief Schedules threads to be run on the CPU
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cassert>
#include <cstring>
#include <functional>
#include <iterator>
#include <list>
#include <queue>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physical_memory.h>
#include <kernel/arch/x86_64/scheduler.h>
#include <kernel/debug.h>

#define IRQ_PIT_TIMER 32
#define IRQ_SCHED_YIELD 48

extern "C" void scheduler_preempt(CPU::StackFrame *);
extern "C" void scheduler_yield(CPU::StackFrame *);

static std::list<Scheduler::Thread> threads;
static std::list<Scheduler::Thread>::iterator current_thread;

auto cmp = [](Scheduler::Thread *a, Scheduler::Thread *b) { return a->sleep_until > b->sleep_until; };
static std::priority_queue<Scheduler::Thread *, std::vector<Scheduler::Thread *>, decltype(cmp)> sleep_queue(cmp);

static uint64_t current_tick = 0;

namespace Scheduler {
	/**
	 * @brief Determine the next thread to run
	 *
	 * @return The next thread to run
	 */
	static Thread &schedule() {
		while (!sleep_queue.empty()) {
			auto &thread = sleep_queue.top();
			if (thread->sleep_until > current_tick) {
				break;
			}
			thread->status = Thread::Status::WAITING;
			sleep_queue.pop();
		}

		auto last_thread = current_thread;

		do {
			std::advance(current_thread, 1);
			if (current_thread == threads.end()) {
				current_thread = threads.begin();
			}
			if (current_thread == last_thread) {
				break;
			}
		} while (current_thread->status != Thread::Status::WAITING);

		return *current_thread;
	}

	/**
	 * @brief Wrapper function to start a thread
	 *
	 * @param entry The entry point of the thread
	 */
	static void thread_wrapper(void (*entry)(void)) {
		std::invoke(entry);
		current_thread->status = Thread::Status::STOPPED;
		yield();
	}
}

void Scheduler::init(void) {
	Debug::log("Initializing scheduler...");
	Interrupts::set_isr(IRQ_PIT_TIMER, scheduler_preempt);
	Interrupts::set_isr(IRQ_SCHED_YIELD, scheduler_yield);
	// TODO change PIT IRQ frequency

	threads.emplace_back();
	threads.back().id = Thread::alloc_id();
	threads.back().status = Thread::Status::RUNNING;

	Debug::log_ok("Scheduler initialized");
}

void Scheduler::start(void) {
	Debug::log("Starting scheduler...");
	assert(!threads.empty());
	current_thread = threads.begin();

	PIC::clear_mask(0);
	Interrupts::enable();

	while (true) {
		for (auto thread = threads.begin(); thread != threads.end();) {
			if (thread->status == Thread::Status::STOPPED) {
				auto stack = Memory::Paging::translate(thread->stack_base);
				assert(stack.has_value());
				Memory::PhysicalMemory::free(stack.value());
				thread = threads.erase(thread);
			} else {
				++thread;
			}
		}

		yield();
	}
}

Scheduler::Thread *Scheduler::create_thread(void (*entry)(void)) {
	Thread thread{};

	auto stack = Memory::PhysicalMemory::alloc();
	assert(stack.has_value());

	thread.id = Thread::alloc_id();
	thread.status = Thread::Status::WAITING;
	thread.stack_base = Memory::Paging::to_kernel(stack.value());

	thread.regs.rdi = reinterpret_cast<uint64_t>(entry);
	thread.regs.frame.rip = reinterpret_cast<uint64_t>(thread_wrapper);
	thread.regs.frame.rflags = RFLAGS_RESERVED | RFLAGS_INTERRUPT_ENABLE;
	thread.regs.frame.cs = GDT_KCODE;
	thread.regs.frame.ss = GDT_KDATA;
	thread.regs.frame.rsp = thread.stack_base + Memory::Paging::PAGE_SIZE;

	threads.push_back(thread);
	return &threads.back();
}

void Scheduler::sleep_until(uint64_t tick) {
	current_thread->sleep_until = tick;
	current_thread->status = Thread::Status::SLEEPING;
	sleep_queue.push(&*current_thread);
	yield();
}

void Scheduler::sleep_for(uint64_t ticks) {
	sleep_until(current_tick + ticks);
}

void Scheduler::yield(void) {
	Interrupts::invoke<IRQ_SCHED_YIELD>();
}

const Scheduler::Thread *Scheduler::Thread::current(void) {
	return &*current_thread;
}

#pragma GCC push_options
#pragma GCC target("general-regs-only")

/**
 * @brief Switch the CPU context to the next thread
 *
 * @param state A pointer to the CPU state on the stack
 *
 * @details This function is called by either the scheduler_preempt or scheduler_yield interrupt handler. Just before
 * this function is called, the CPU state is pushed onto the stack. This function will then modify the CPU state to
 * switch to the next thread. When this function returns, the CPU state will be popped off the stack and the next
 * thread will begin executing.
 */
extern "C" void __attribute__((no_caller_saved_registers)) scheduler_swap(CPU::State *state) {
	using namespace Scheduler;

	PIC::eoi(0);
	auto &current = *current_thread;
	auto &next = schedule();

	if (current == next) {
		return;
	}

	// TODO save FPU, CR3, etc
	// save CPU state registers
	memcpy(&current.regs, state, sizeof(CPU::State));
	if (current.status == Thread::Status::RUNNING) {
		current.status = Thread::Status::WAITING;
	}

	// TODO restore FPU, CR3, etc
	// restore CPU state registers
	memcpy(state, &next.regs, sizeof(CPU::State));
	next.status = Thread::Status::RUNNING;
}

/**
 * @brief Update the current tick count
 *
 */
extern "C" void __attribute__((no_caller_saved_registers)) scheduler_tick(void) {
	current_tick++;
}

#pragma GCC pop_options