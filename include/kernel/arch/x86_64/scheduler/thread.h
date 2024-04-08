/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-18
 * @brief Represents a thread to be run on the CPU
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
#include <kernel/arch/x86_64/memory/virtaddr.h>

namespace Scheduler {
	/**
	 * @brief Represents a thread to be run on the CPU
	 *
	 */
	class Thread {
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
		Memory::VirtAddr stack_base;

		uint64_t sleep_until; // TODO use std::chrono

		// TODO other fields

		/**
		 * @brief Check if two threads are equal
		 *
		 * @param other The other task to compare to
		 * @return true if the threads are equal, false otherwise
		 */
		[[nodiscard]] bool operator==(const Thread &other) const {
			return id == other.id;
		}

		/**
		 * @brief Allocate a unique thread ID
		 *
		 * @return The unique thread ID
		 */
		static size_t alloc_id() {
			// TODO better ID allocation
			static size_t id = 0;
			return id++;
		}
	};
}