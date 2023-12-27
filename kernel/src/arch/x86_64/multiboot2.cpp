/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief Various structures and functions for reading multiboot2 boot info
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdint.h>

#include <assert.h>

#include <arch/x86_64/cpu.h>
#include <arch/x86_64/multiboot2.h>
#include <debug.h>

static void *multiboot2_info;
static uint32_t total_size;

void Multiboot2::init(uint32_t magic, void *addr) {
	Debug::log("Initializing multiboot2 info block...");
	assert(magic == MULTIBOOT2_MAGIC);

	multiboot2_info = addr;
	total_size = *static_cast<uint32_t *>(addr);
	Debug::log_ok("Multiboot2 info block initialized");
}

void const *Multiboot2::get_entry(BootInfoType type) {
	uint32_t offset = 8;

	while (offset < total_size) {
		auto entry_type = *reinterpret_cast<BootInfoType *>(static_cast<uint8_t *>(multiboot2_info) + offset);
		auto entry_size = *reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(multiboot2_info) + offset + 4);

		if (entry_type == type) {
			return static_cast<uint8_t *>(multiboot2_info) + offset;
		}
		if (entry_type == BootInfoType::END) {
			break;
		}

		// allign to 8 bytes
		offset += ((entry_size + 7) & ~0b111);
	}

	return nullptr;
}