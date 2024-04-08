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

#pragma once

#include <stddef.h>

#include <chrono>

#include <kernel/arch/x86_64/scheduler/thread.h>

namespace Scheduler {
	/**
	 * @brief Initialize the scheduler
	 *
	 */
	void init(void);

	/**
	 * @brief Starts the scheduler
	 *
	 * @note This function will not return
	 */
	[[noreturn]] void start(void);

	/**
	 * @brief Create a new task to be scheduled
	 *
	 * @param entry The entry point for the task
	 */
	void create_thread(void (*entry)(void));

	/**
	 * @brief Put the current task to sleep until a given time
	 *
	 * @param duration The time to sleep until
	 */
	void sleep_until(std::chrono::nanoseconds duration);
	// FIXME probably should be a time_point instead of a duration

	/**
	 * @brief Put the current task to sleep for a given amount of time
	 *
	 * @param duration The time to sleep for
	 */
	void sleep_for(std::chrono::nanoseconds duration);

	/**
	 * @brief Yield the current task
	 *
	 */
	void yield(void);
}