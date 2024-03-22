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

#define CPUID_EAX 0
#define CPUID_EBX 1
#define CPUID_ECX 2
#define CPUID_EDX 3

#define CPUID_FEATURE(leaf, bit, reg) ((leaf << 8) | (bit << 2) | reg)

// Feature Format:
// bits[0-1]: register select (eax = 0, ebx = 1, ecx = 2, edx = 3)
// bits[2-8]: bit position
// bits[9-15]: cpuid leaf

namespace CPU {
	/**
	 * @brief Function stack frame
	 *
	 */
	struct StackFrame {
		uint64_t rip;
		uint64_t cs;
		uint64_t rflags;
		uint64_t rsp;
		uint64_t ss;
	} PACKED;

	/**
	 * @brief CPU state
	 *
	 */
	struct State {
		uint64_t rsp;
		uint64_t rbp;
		uint64_t rax;
		uint64_t rbx;
		uint64_t rcx;
		uint64_t rdx;
		uint64_t rsi;
		uint64_t rdi;
		uint64_t r8;
		uint64_t r9;
		uint64_t r10;
		uint64_t r11;
		uint64_t r12;
		uint64_t r13;
		uint64_t r14;
		uint64_t r15;
		StackFrame frame;
	} PACKED;

	/**
	 * @brief Possible CPU features
	 *
	 */
	enum class Feature : uint16_t {
		RDSEED = CPUID_FEATURE(7, 18, CPUID_EBX),
		RDRAND = CPUID_FEATURE(1, 30, CPUID_ECX),
		RDTSC = CPUID_FEATURE(1, 4, CPUID_EDX),
		SSE = CPUID_FEATURE(1, 25, CPUID_EDX),
		SSE2 = CPUID_FEATURE(1, 26, CPUID_EDX),
		SSE3 = CPUID_FEATURE(1, 0, CPUID_ECX),
		SSSE3 = CPUID_FEATURE(1, 9, CPUID_ECX),
		SSE4_1 = CPUID_FEATURE(1, 19, CPUID_ECX),
		SSE4_2 = CPUID_FEATURE(1, 20, CPUID_ECX),
		AVX = CPUID_FEATURE(1, 28, CPUID_ECX),
		AVX2 = CPUID_FEATURE(7, 5, CPUID_EBX)
	};
	// TODO Add more features

	/**
	 * @brief Checks if the CPU has the specified feature
	 *
	 * @param feature The feature to check for
	 * @return true if the CPU has the feature, false otherwise
	 */
	bool has_feature(Feature feature);

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

	/**
	 * @brief Get the value of the RFLAGS register
	 *
	 * @return The value of the RFLAGS register
	 */
	[[nodiscard]] inline uint64_t get_flags(void) {
		uint64_t flags = 0;
		asm volatile("pushfq; popq %0" : "=r"(flags));
		return flags;
	}
}
