/**
 * @file kernel/kmain.cpp
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

#include <kernel/multiboot2.hpp>
#include <kernel/uart.hpp>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36D76289

void kprintf(const char *text) {
	UART comm(UART::COM1);

	char *str = (char *)text;
	while (*str != '\0') {
		comm.write(*str);
		str++;
	}
}

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
	kprintf("\nperforming system checks...\n");

	kprintf("> checking multiboot2 magic number: ");
	ASSERT(MULTIBOOT2_MAGIC, magic);

	kprintf("> parsing multiboot2 info block: ");
	ASSERT(true, Multiboot2::init(addr));

	auto bootloader_name = Multiboot2::getPtr<char>(Multiboot2::BOOTLOADER_NAME);
	auto boot_cmd_line = Multiboot2::getPtr<char>(Multiboot2::BOOT_CMD_LINE);

	kprintf("> booted via: \u001b[36m\"");
	kprintf(bootloader_name);
	kprintf("\"\u001b[0m\n");
	kprintf("> grub options: \u001b[36m\"");
	kprintf(boot_cmd_line);
	kprintf("\"\u001b[0m\n");

	while (true) {
		// spin-lock
	}

	return;
}