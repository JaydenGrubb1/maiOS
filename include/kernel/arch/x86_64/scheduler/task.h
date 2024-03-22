/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-18
 * @brief Represents a task (process or thread) to be run on the CPU
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

#include <kernel/arch/x86_64/cpu.h>

namespace Scheduler {
	/**
	 * @brief Represents a task (process or thread) to be run on the CPU
	 *
	 */
	class Task {
	  public:
		enum class Status {
			Running,
			Waiting,
			Stopped,
			Blocked,
			Sleeping
		};

		size_t id;
		CPU::State regs;
		Status status;

		// TODO other fields

		/**
		 * @brief Check if two tasks are equal
		 *
		 * @param other The other task to compare to
		 * @return true if the tasks are equal, false otherwise
		 */
		[[nodiscard]] bool operator==(const Task &other) const {
			return id == other.id;
		}
	};
}