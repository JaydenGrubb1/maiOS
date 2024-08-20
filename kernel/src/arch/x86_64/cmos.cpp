/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Provides access to the CMOS
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/cmos.h>
#include <kernel/arch/x86_64/io.h>

uint8_t CMOS::read(uint8_t reg) {
	IO::write<uint8_t>(0x70, reg);
	return IO::read<uint8_t>(0x71);
}

void CMOS::write(uint8_t reg, uint8_t value) {
	IO::write<uint8_t>(0x70, reg);
	IO::write<uint8_t>(0x71, value);
}