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
#include <iterator>
#include <list>
#include <queue>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physical_memory.h>
#include <kernel/arch/x86_64/scheduler.h>
#include <kernel/debug.h>

extern "C" void switch_thread(CPU::StackFrame *);

static std::list<Scheduler::Thread> threads;
static std::list<Scheduler::Thread>::iterator current_thread;

auto cmp = [](Scheduler::Thread *a, Scheduler::Thread *b) { return a->sleep_until > b->sleep_until; };
static std::priority_queue<Scheduler::Thread *, std::vector<Scheduler::Thread *>, decltype(cmp)> sleep_queue(cmp);

// TODO move to time module
static uint64_t current_ns(void) {
	uint32_t lo, hi;
	asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
	uint64_t tsc = static_cast<uint64_t>(hi) << 32 | lo;
	return tsc * 10 / 36;
	// HACK rough estimate of ns, assumes 3.6 GHz CPU
}

namespace Scheduler {
	/**
	 * @brief Determine the next thread to run
	 *
	 * @return The next thread to run
	 */
	static Thread &schedule() {
		while (!sleep_queue.empty()) {
			auto &thread = sleep_queue.top();
			if (thread->sleep_until > current_ns()) {
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
		entry();
		current_thread->status = Thread::Status::STOPPED;
		yield();
	}
}

void Scheduler::init(void) {
	Debug::log("Initializing scheduler...");
	Interrupts::set_isr(32, switch_thread);

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

void Scheduler::create_thread(void (*entry)(void)) {
	Thread thread;
	memset(&thread, 0, sizeof(Thread));

	auto stack = Memory::PhysicalMemory::alloc();
	assert(stack.has_value());

	thread.id = Thread::alloc_id();
	thread.status = Thread::Status::WAITING;
	thread.stack_base = Memory::Paging::to_kernel(stack.value());

	thread.regs.rdi = reinterpret_cast<uint64_t>(entry);
	thread.regs.frame.rip = reinterpret_cast<uint64_t>(thread_wrapper);
	thread.regs.frame.rflags = 0x202;
	thread.regs.frame.cs = 0x08;
	thread.regs.frame.ss = 0x10;
	thread.regs.frame.rsp = thread.stack_base + Memory::Paging::PAGE_SIZE;

	threads.push_back(thread);
}

void Scheduler::sleep_until(std::chrono::nanoseconds duration) {
	auto &thread = *current_thread;
	thread.sleep_until = duration.count();
	thread.status = Thread::Status::SLEEPING;
	sleep_queue.push(&thread);
	yield();
}

void Scheduler::sleep_for(std::chrono::nanoseconds duration) {
	auto end = current_ns() + duration.count();
	sleep_until(std::chrono::nanoseconds(end));
}

void Scheduler::yield(void) {
	Interrupts::invoke<32>();
}

const Scheduler::Thread *Scheduler::Thread::current(void) {
	return &*current_thread;
}

/**
 * @brief Switch the CPU context to the next thread
 *
 * @param state A pointer to the CPU state on the stack
 *
 * @details This function is called by the thread switch interrupt handler. Just before this function is called,
 * the CPU state is pushed onto the stack. This function will then modify the CPU state to switch to the next thread.
 * When this function returns, the CPU state will be popped off the stack and the next thread will begin executing.
 */
#pragma GCC push_options
#pragma GCC target("general-regs-only")
extern "C" void __attribute__((no_caller_saved_registers)) switch_context(CPU::State *state) {
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
#pragma GCC pop_options