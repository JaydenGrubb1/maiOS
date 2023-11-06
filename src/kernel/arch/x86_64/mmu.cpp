/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Handles all memory management tasks for the kernel
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/mmu.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>
#include <stddef.h>

void MMU::init(void) {
	// TODO Actually implement memory management

	Debug::log_info("Multiboot2 provided physical memory map:");
	auto mmap = reinterpret_cast<Multiboot2::MemoryMap const *>(Multiboot2::get_entry(Multiboot2::MEMORY_MAP));
	for (size_t i = 0; i < (mmap->size - 16) / mmap->entry_size; i++) {
		auto mem = mmap->entries[i];
		Debug::log("- [mem %#.16lx-%#.16lx] %s",
				   mem.base,
				   mem.base + mem.length,
				   mem.type == Multiboot2::MemoryMapEntryType::AVAILABLE ? "available" : "reserved");
		// TODO Add more types
	}

	Debug::log_warning("MMU is not yet implemented");
}