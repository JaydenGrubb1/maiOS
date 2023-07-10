/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-20
 * @brief Main entry point for the operating system (64-bit)
 *
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/interrupts.hpp>
#include <kernel/arch/x86_64/interrupts/pic.hpp>
#include <kernel/arch/x86_64/multiboot2.hpp>
#include <kernel/logger.hpp>
#include <stdbool.h>
#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36D76289

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, uint8_t *addr) {
	LOG("Booting mai-OS v0.0.1");

	if (magic == MULTIBOOT2_MAGIC) {
		LOG_PASS("Multiboot2 magic number valid: %#.8x", magic);
	} else {
		LOG_FAIL("Multiboot2 magic number invalid: %#.8x", magic);
		return;
	}

	if (Multiboot2::init(addr)) {
		LOG_PASS("Multiboot2 info block initialized");
	} else {
		LOG_FAIL("Multiboot2 info block failed to initialize");
		return;
	}

	auto bootloader_name = Multiboot2::getPtr<char>(Multiboot2::BOOTLOADER_NAME);
	auto boot_cmd_line = Multiboot2::getPtr<char>(Multiboot2::BOOT_CMD_LINE);

	LOG_INFO("Booted via: %s", bootloader_name);
	LOG_INFO("GRUB options: %s", boot_cmd_line);

	PIC::init();
	Interrupts::init_idt();
	Interrupts::sti();

	LOG_INFO("Entering idle loop...");
	while (true) {
		// spin-lock
	}

	return;
}