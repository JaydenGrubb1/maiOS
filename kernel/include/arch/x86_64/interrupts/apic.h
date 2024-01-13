/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

namespace APIC {
	/**
	 * @brief Local APIC registers
	 *
	 */
	enum class Register : uint16_t {
		ID = 0x20,			   // APIC ID
		VERSION = 0x30,		   // APIC version
		TPR = 0x80,			   // Task Priority Register
		APR = 0x90,			   // Arbitration Priority Register
		PPR = 0xa0,			   // Processor Priority Register
		EOI = 0xb0,			   // End of Interrupt
		RRD = 0xc0,			   // Remote Read Register
		LDR = 0xd0,			   // Logical Destination Register
		DFR = 0xe0,			   // Destination Format Register
		SVR = 0xf0,			   // Spurious Interrupt Vector Register
		ISR0 = 0x100,		   // In-Service Register 0
		ISR1 = 0x110,		   // In-Service Register 1
		ISR2 = 0x120,		   // In-Service Register 2
		ISR3 = 0x130,		   // In-Service Register 3
		ISR4 = 0x140,		   // In-Service Register 4
		ISR5 = 0x150,		   // In-Service Register 5
		ISR6 = 0x160,		   // In-Service Register 6
		ISR7 = 0x170,		   // In-Service Register 7
		TMR0 = 0x180,		   // Trigger Mode Register 0
		TMR1 = 0x190,		   // Trigger Mode Register 1
		TMR2 = 0x1a0,		   // Trigger Mode Register 2
		TMR3 = 0x1b0,		   // Trigger Mode Register 3
		TMR4 = 0x1c0,		   // Trigger Mode Register 4
		TMR5 = 0x1d0,		   // Trigger Mode Register 5
		TMR6 = 0x1e0,		   // Trigger Mode Register 6
		TMR7 = 0x1f0,		   // Trigger Mode Register 7
		IRR0 = 0x200,		   // Interrupt Request Register 0
		IRR1 = 0x210,		   // Interrupt Request Register 1
		IRR2 = 0x220,		   // Interrupt Request Register 2
		IRR3 = 0x230,		   // Interrupt Request Register 3
		IRR4 = 0x240,		   // Interrupt Request Register 4
		IRR5 = 0x250,		   // Interrupt Request Register 5
		IRR6 = 0x260,		   // Interrupt Request Register 6
		IRR7 = 0x270,		   // Interrupt Request Register 7
		ESR = 0x280,		   // Error Status Register
		CMCI = 0x2f0,		   // Corrected Machine Check Interrupt
		ICR1 = 0x300,		   // Interrupt Command Register 1
		ICR2 = 0x310,		   // Interrupt Command Register 2
		LVT_TIMER = 0x320,	   // Local Vector Table Timer
		LVT_THERMAL = 0x330,   // Local Vector Table Thermal Sensor
		LVT_PERFMON = 0x340,   // Local Vector Table Performance Monitoring
		LVT_LINT0 = 0x350,	   // Local Vector Table LINT0
		LVT_LINT1 = 0x360,	   // Local Vector Table LINT1
		LVT_ERROR = 0x370,	   // Local Vector Table Error
		INIT_COUNT = 0x380,	   // Initial Count Register
		CURRENT_COUNT = 0x390, // Current Count Register
		DCR = 0x3E0,		   // Divide Configuration Register
	};

	/**
	 * @brief Initializes the Local APIC
	 *
	 */
	void init(void);

	/**
	 * @brief Reads the value of the specified APIC register
	 *
	 * @param reg The register to read
	 * @return The value of the register
	 */
	uint32_t read(Register reg);

	/**
	 * @brief Writes the value to the specified APIC register
	 *
	 * @param reg The register to write to
	 * @param value The value to write
	 */
	void write(Register reg, uint32_t value);

	/**
	 * @brief Sends an end of interrupt signal to the Local APIC
	 *
	 */
	void eoi(void);
}