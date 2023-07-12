/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/multiboot2v2.h>
#include <kernel/debug.h>
#include <stdint.h>

static void *multiboot2_info;
static uint32_t total_size;

void Multiboot2v2::init(uint32_t magic, void *addr) {
	Debug::log("Initializing multiboot2 info block...");

	if (magic != MULTIBOOT2_MAGIC) {
		Debug::log_failure("Multiboot2 magic number invalid: %#.8x", magic);
		CPU::halt();
	}

	multiboot2_info = addr;
	total_size = *(uint32_t *)addr;
	Debug::log_ok("Multiboot2 info block initialized");
}

void *Multiboot2v2::get_entry(Multiboot2v2::BootInfoType type) {
	uint32_t offset = 8;
	uint32_t entry_type;
	uint32_t entry_size;

	while (offset < total_size) {
		entry_type = *(uint32_t *)((uint8_t *)multiboot2_info + offset);
		entry_size = *(uint32_t *)((uint8_t *)multiboot2_info + offset + 4);

		if (entry_type == type) {
			return (uint8_t *)multiboot2_info + offset;
		}
		if (entry_type == 0) {
			break;
		}

		// allign to 8 bytes
		offset += ((entry_size + 7) & ~0b111);
	}

	return nullptr;
}