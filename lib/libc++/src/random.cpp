/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-30
 * @brief Random number generator for non-deterministic random numbers
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <random>

#include <kernel/arch/cpu.h>
#include <kernel/debug.h>

// TODO remove these
#include <kernel/arch/x86_64/time/rtc.h>
#include <stdlib.h>

namespace std {
	unsigned int random_device::operator()(void) {
		// TODO move to <kernel/random.h> or something
		if (CPU::has_feature(CPU::Feature::RDSEED)) {
			unsigned int value;
			asm volatile("1: rdseed %0; jnc 1b" : "=r"(value));
			return value;
		} else if (CPU::has_feature(CPU::Feature::RDRAND)) {
			unsigned int value;
			asm volatile("1: rdrand %0; jnc 1b" : "=r"(value));
			return value;
		} else {
			Debug::log_warning("RDSEED and RDRAND not supported, using rand()");
			// TODO add DateTime.epoch() or something instead of this
			auto time = Time::RTC::now();
			srand(*reinterpret_cast<uint64_t *>(&time) ^ rand());
			return rand();
		}
	}
}