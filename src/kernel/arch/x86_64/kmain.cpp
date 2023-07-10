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
#include <kernel/kprintf.hpp>
#include <stdbool.h>
#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36D76289

#define ASSERT(EXPECTED, ACTUAL)                              \
	if (EXPECTED == ACTUAL)                                   \
		kprintf("\u001b[32m[success]\u001b[0m\n");            \
	else {                                                    \
		kprintf("\u001b[31m[failed]\u001b[0m\nexiting...\n"); \
		return;                                               \
	}

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, uint8_t *addr) {
	kprintf("\nbooting maiOS...\n");

	kprintf("> checking multiboot2 magic number: ");
	ASSERT(MULTIBOOT2_MAGIC, magic);
	kprintf("> parsing multiboot2 info block: ");
	ASSERT(true, Multiboot2::init(addr));

	auto bootloader_name = Multiboot2::getPtr<char>(Multiboot2::BOOTLOADER_NAME);
	auto boot_cmd_line = Multiboot2::getPtr<char>(Multiboot2::BOOT_CMD_LINE);

	kprintf("> booted via: \u001b[36m\"%s\"\u001b[0m\n", bootloader_name);
	kprintf("> grub options: \u001b[36m\"%s\"\u001b[0m\n", boot_cmd_line);

	PIC::init();
	Interrupts::init_idt();
	Interrupts::sti();

	while (true) {
		// spin-lock
	}

	return;
}