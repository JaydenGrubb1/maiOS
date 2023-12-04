/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-10
 * @brief Various functions for interacting with the Programmable Interrupt Controller
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/io.h>
#include <kernel/debug.h>

#define MASTER_PIC_CMD 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC_CMD 0xA0
#define SLAVE_PIC_DATA 0xA1

#define ICW1_ICW4 0x01		// Indicates that ICW4 will be present
#define ICW1_SINGLE 0x02	// Single (cascade) mode
#define ICW1_INTERVAL4 0x04 // Call address interval 4 (8)
#define ICW1_LEVEL 0x08		// Level triggered (edge) mode
#define ICW1_INIT 0x10		// Initialization - required!

#define ICW4_8086 0x01		 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO 0x02		 // Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08	 // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C // Buffered mode/master
#define ICW4_SFNM 0x10		 // Special fully nested (not)

#define PIC_EOI 0x20

void PIC::init(void) {
	Debug::log("Initializing PIC...");

	// remap PIC to avoid conflicts with CPU exceptions
	remap(0x20, 0x28);

	// mask all interrupts
	IO::out8(MASTER_PIC_DATA, 0xFB);
	IO::out8(SLAVE_PIC_DATA, 0xFF);

	Debug::log_ok("PIC initialized");
}

void PIC::remap(uint8_t master, uint8_t slave) {
	Debug::log_info("Remapping PIC offset: master = %#.2x, slave = %#.2x", master, slave);

	// save masks
	uint8_t master_mask = IO::in8(MASTER_PIC_DATA);
	uint8_t slave_mask = IO::in8(SLAVE_PIC_DATA);

	// starts the initialization sequence in cascade mode
	IO::out8(MASTER_PIC_CMD, ICW1_INIT | ICW1_ICW4);
	IO::out8(SLAVE_PIC_CMD, ICW1_INIT | ICW1_ICW4);

	// set the vector offsets
	IO::out8(MASTER_PIC_DATA, master);
	IO::out8(SLAVE_PIC_DATA, slave);

	// tell Master PIC that there is a slave PIC at IRQ2
	IO::out8(MASTER_PIC_CMD, 4);
	IO::out8(SLAVE_PIC_DATA, 2);

	// tell PICs to operate in 8086 mode
	IO::out8(MASTER_PIC_DATA, ICW4_8086);
	IO::out8(SLAVE_PIC_DATA, ICW4_8086);

	// restore saved masks
	IO::out8(MASTER_PIC_DATA, master_mask);
	IO::out8(SLAVE_PIC_DATA, slave_mask);
}

void PIC::eoi(uint8_t irq) {
	if (irq >= 8)
		IO::out8(SLAVE_PIC_CMD, PIC_EOI);

	IO::out8(MASTER_PIC_CMD, PIC_EOI);
}

bool PIC::set_mask(uint8_t irq) {
	uint16_t port;
	uint8_t value;

	if (irq > 15) {
		Debug::log_failure("Cannot set mask for invalid IRQ: %#.2x", irq);
		return false;
	}

	if (irq < 8) {
		port = MASTER_PIC_DATA;
	} else {
		port = SLAVE_PIC_DATA;
		irq -= 8;
	}

	value = IO::in8(port) | (1 << irq);
	IO::out8(port, value);
	return true;
}

bool PIC::clear_mask(uint8_t irq) {
	uint16_t port;
	uint8_t value;

	if (irq > 15) {
		Debug::log_failure("Cannot clear mask for invalid IRQ: %#.2x", irq);
		return false;
	}

	if (irq < 8) {
		port = MASTER_PIC_DATA;
	} else {
		port = SLAVE_PIC_DATA;
		irq -= 8;
	}

	value = IO::in8(port) & ~(1 << irq);
	IO::out8(port, value);
	return true;
}