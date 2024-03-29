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

extern "C" [[noreturn]] void __start_tasks(CPU::State *);
extern "C" void __switch_tasks(CPU::StackFrame *);

static std::list<Scheduler::Task> tasks;
static std::list<Scheduler::Task>::iterator current_task;

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
		auto next = current_task;
		do {
			next = std::next(next);
			if (next == tasks.end()) {
				next = tasks.begin();
			}
		} while (next->status != Task::Status::Waiting);
		return {*current_task, *next};
	}

	/**
	 * @brief Wrapper function to start a task
	 *
	 * @param entry The entry point of the task
	 */
	static void task_wrapper(void (*entry)(void)) {
		entry();
		current_task->status = Task::Status::Stopped;
		yield();
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
		memcpy(&current.regs, state, sizeof(CPU::State));
		if (current.status == Task::Status::Running) {
			current.status = Task::Status::Waiting;
		}

		// TODO restore FPU, CR3, etc
		// restore CPU state registers
		memcpy(state, &next.regs, sizeof(CPU::State));
		next.status = Task::Status::Running;
	}
}

void Scheduler::init(void) {
	Debug::log("Initializing scheduler...");
	Interrupts::set_isr(32, __switch_tasks);
	PIC::clear_mask(0);
	Debug::log_ok("Scheduler initialized");
}

void Scheduler::start(void) {
	Debug::log("Starting scheduler...");
	assert(!tasks.empty());
	current_task = tasks.begin();
	current_task->status = Task::Status::Running;
	__start_tasks(&current_task->regs);
}

void Scheduler::create_task(void (*entry)(void)) {
	Task task;
	memset(&task, 0, sizeof(Task));

	auto stack = Memory::PhysicalMemory::alloc();
	assert(stack.has_value());
	auto stack_virt = Memory::Paging::to_kernel(stack.value());

	task.id = alloc_id();
	task.status = Task::Status::Waiting;

	task.regs.rdi = reinterpret_cast<uint64_t>(entry);
	task.regs.frame.rip = reinterpret_cast<uint64_t>(task_wrapper);
	task.regs.frame.rflags = 0x202;
	task.regs.frame.cs = 0x08;
	task.regs.frame.ss = 0x10;
	task.regs.frame.rsp = stack_virt + Memory::Paging::PAGE_SIZE;

	tasks.push_back(task);
}

void Scheduler::yield(void) {
	asm volatile("int 32");
}