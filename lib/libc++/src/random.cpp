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

// TODO remove these
#include <kernel/arch/x86_64/time/rtc.h>
#include <stdlib.h>

namespace std {
	unsigned int random_device::operator()() {
		// HACK replace with a real random number generator
		srand(rand() ^ Time::RTC::now().second);
		return rand();
	}
}