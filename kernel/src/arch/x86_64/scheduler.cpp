/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-18
 * @brief Schedules tasks (process or thread) to be run on the CPU
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cassert>
#include <cstring>
#include <pair>
#include <vector>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physical_memory.h>
#include <kernel/arch/x86_64/scheduler.h>
#include <kernel/debug.h>

extern "C" [[noreturn]] void __start_tasks(CPU::State *);
extern "C" void __switch_tasks(CPU::StackFrame *);

// TODO use different data structure
static std::vector<Scheduler::Task> tasks;
static size_t current_task = 0;

namespace Scheduler {
	/**
	 * @brief Allocate a unique task ID
	 *
	 * @return The unique task ID
	 */
	size_t alloc_id() {
		// TODO better ID allocation
		static size_t id = 0;
		return id++;
	}

	/**
	 * @brief Determine the next task to run
	 *
	 * @return The current and next task to run
	 */
	static std::pair<Task &, Task &> schedule() {
		// TODO use better scheduling algorithm
		Task &current = tasks[current_task];
		current_task++;
		if (current_task >= tasks.size()) {
			current_task = 0;
		}
		Task &next = tasks[current_task];
		return {current, next};
	}

	/**
	 * @brief Switch the CPU context to the next task
	 *
	 * @param state A pointer to the CPU state on the stack
	 *
	 * @details This function is called by the task switch interrupt handler. Just before this function is called,
	 * the CPU state is pushed onto the stack. This function will then modify the CPU state to switch to the next task.
	 * When this function returns, the CPU state will be popped off the stack and the next task will begin executing.
	 */
	extern "C" void __attribute__((no_caller_saved_registers)) switch_context(CPU::State *state) {
		PIC::eoi(0);
		auto [current, next] = schedule();

		if (current == next) {
			return;
		}

		// TODO save FPU, CR3, etc
		// save CPU state registers
		memcpy(&current.state, state, sizeof(CPU::State));

		// TODO restore FPU, CR3, etc
		// restore CPU state registers
		memcpy(state, &next.state, sizeof(CPU::State));
	}
}

void Scheduler::init(void) {
	Debug::log("Initializing scheduler...");
	tasks.reserve(16);

	Interrupts::set_isr(32, __switch_tasks);
	PIC::clear_mask(0);
	Debug::log_ok("Scheduler initialized");
}

void Scheduler::start(void) {
	Debug::log("Starting scheduler...");
	__start_tasks(&tasks.front().state);
}

void Scheduler::create_task(void (*entry)(void)) {
	assert(tasks.size() < 16); // TODO remove when using different data structure

	Task task;
	memset(&task, 0, sizeof(Task));

	auto stack = Memory::PhysicalMemory::alloc();
	assert(stack.has_value());
	auto stack_virt = Memory::Paging::to_kernel(stack.value());
	assert(Memory::Paging::translate(stack_virt).has_value());

	task.id = alloc_id();

	task.state.frame.rip = reinterpret_cast<uint64_t>(entry);
	task.state.frame.rflags = 0x202;
	task.state.frame.cs = 0x08;
	task.state.frame.ss = 0x10;
	task.state.frame.rsp = stack_virt + Memory::Paging::PAGE_SIZE;

	tasks.push_back(task);
}

void Scheduler::yield(void) {
	asm volatile("int 32");
}