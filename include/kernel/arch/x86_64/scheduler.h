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

#include <kernel/arch/x86_64/scheduler/task.h>

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
	void start(void);

	/**
	 * @brief Create a new task to be scheduled
	 *
	 * @param entry The entry point for the task
	 */
	void create_task(void (*entry)(void));

	/**
	 * @brief Yield the current task
	 *
	 */
	void yield(void);
}