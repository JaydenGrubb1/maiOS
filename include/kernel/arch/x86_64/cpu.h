/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief CPU related functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

#include <kernel/arch/x86_64/msr.h>
#include <kernel/defines.h>

namespace CPU {
	/**
	 * @brief Halts the CPU
	 *
	 */
	inline NORETURN void halt(void) {
		asm volatile("cli; hlt");
		__builtin_unreachable();
	}

	/**
	 * @brief Get the value of the specified Model Specific Register (MSR)
	 *
	 * @param msr The MSR to read
	 * @return The value of the MSR
	 */
	[[nodiscard]] inline uint64_t get_msr(uint32_t msr) {
		uint32_t lo, hi;
		asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
		return (static_cast<uint64_t>(hi) << 32) | lo;
	}

	/**
	 * @brief Set the value of the specified Model Specific Register (MSR)
	 *
	 * @param msr The MSR to write
	 * @param value The value to write to the MSR
	 */
	inline void set_msr(uint32_t msr, uint64_t value) {
		uint32_t lo = value & 0xFFFFFFFF;
		uint32_t hi = value >> 32;
		asm volatile("wrmsr" ::"a"(lo), "d"(hi), "c"(msr));
	}
}
