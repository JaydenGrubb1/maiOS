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
#include <pair>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physical_memory.h>
#include <kernel/arch/x86_64/scheduler.h>
#include <kernel/debug.h>

extern "C" [[noreturn]] void start_threads(CPU::State *);
extern "C" void switch_thread(CPU::StackFrame *);

static std::list<Scheduler::Thread> threads;
static std::list<Scheduler::Thread>::iterator current_thread;

namespace Scheduler {
	/**
	 * @brief Allocate a unique thread ID
	 *
	 * @return The unique thread ID
	 */
	size_t alloc_id() {
		// TODO better ID allocation
		static size_t id = 0;
		return id++;
	}

	/**
	 * @brief Determine the next thread to run
	 *
	 * @return The next thread to run
	 */
	static Thread &schedule() {
		do {
			std::advance(current_thread, 1);
			if (current_thread == threads.end()) {
				current_thread = threads.begin();
			}
		} while (current_thread->status != Thread::Status::Waiting);
		return *current_thread;
	}

	/**
	 * @brief Wrapper function to start a thread
	 *
	 * @param entry The entry point of the thread
	 */
	static void thread_wrapper(void (*entry)(void)) {
		entry();
		current_thread->status = Thread::Status::Stopped;
		yield();
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
	extern "C" void __attribute__((no_caller_saved_registers)) switch_context(CPU::State *state) {
		PIC::eoi(0);
		auto &current = *current_thread;
		auto &next = schedule();

		if (current == next) {
			return;
		}

		// TODO save FPU, CR3, etc
		// save CPU state registers
		memcpy(&current.regs, state, sizeof(CPU::State));
		if (current.status == Thread::Status::Running) {
			current.status = Thread::Status::Waiting;
		}

		// TODO restore FPU, CR3, etc
		// restore CPU state registers
		memcpy(state, &next.regs, sizeof(CPU::State));
		next.status = Thread::Status::Running;
	}

	/**
	 * @brief The idle thread
	 *
	 */
	[[noreturn]] void idle(void) {
		Debug::log_info("Starting idle thread");
		PIC::clear_mask(0);

		while (true) {
			// TODO clean up stopped threads
			yield();
		}
	}
}

void Scheduler::init(void) {
	Debug::log("Initializing scheduler...");
	Interrupts::set_isr(32, switch_thread);
	create_thread(Scheduler::idle);
	Debug::log_ok("Scheduler initialized");
}

void Scheduler::start(void) {
	Debug::log("Starting scheduler...");
	assert(!threads.empty());
	current_thread = threads.begin();
	current_thread->status = Thread::Status::Running;
	start_threads(&current_thread->regs);
}

void Scheduler::create_thread(void (*entry)(void)) {
	Thread thread;
	memset(&thread, 0, sizeof(Thread));

	auto stack = Memory::PhysicalMemory::alloc();
	assert(stack.has_value());
	auto stack_virt = Memory::Paging::to_kernel(stack.value());

	thread.id = alloc_id();
	thread.status = Thread::Status::Waiting;

	thread.regs.rdi = reinterpret_cast<uint64_t>(entry);
	thread.regs.frame.rip = reinterpret_cast<uint64_t>(thread_wrapper);
	thread.regs.frame.rflags = 0x202;
	thread.regs.frame.cs = 0x08;
	thread.regs.frame.ss = 0x10;
	thread.regs.frame.rsp = stack_virt + Memory::Paging::PAGE_SIZE;

	threads.push_back(thread);
}

void Scheduler::yield(void) {
	asm volatile("int 32");
}