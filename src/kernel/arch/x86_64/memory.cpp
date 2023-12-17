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

#include <defines.h>
#include <stddef.h>

#include <lib/libc++/optional.h>
#include <lib/libc++/vector.h>
#include <lib/libc/string.h>

#include <kernel/arch/x86_64/memory.h>
#include <kernel/arch/x86_64/memory/frame_allocator.h>
#include <kernel/arch/x86_64/memory/memory_region.h>
#include <kernel/arch/x86_64/memory/page_table.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physaddr.h>
#include <kernel/arch/x86_64/memory/virtaddr.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

#define KERNEL_HEAP_SIZE (64 * MiB)

static SECTION(".heap") uint8_t heap[KERNEL_HEAP_SIZE];
static uint8_t *heap_ptr = heap;

static kstd::vector<Memory::MemoryRegion> memory_regions;

void Memory::init(void) {
	Debug::log("Initializing memory...");
	Debug::log_info("Multiboot2 provided physical memory map:");

	auto mmap = reinterpret_cast<Multiboot2::MemoryMap const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::MEMORY_MAP));
	auto entry_count = (mmap->size - 16) / mmap->entry_size;
	memory_regions.reserve(entry_count);

	for (size_t i = 0; i < entry_count; i++) {
		auto mem = mmap->entries[i];
		Debug::log("- [mem %#.16lx-%#.16lx] : %d", mem.base, mem.base + mem.length, static_cast<int>(mem.type));

		if (mem.type == Multiboot2::MemoryMapEntryType::AVAILABLE) {
			memory_regions.emplace_back(Paging::round_up(mem.base), Paging::round_down(mem.base + mem.length));
		}
	}

	FrameAllocator::init(memory_regions);

	// remap the first 2 MiB huge page into 512 x 4 KiB pages except for the first page (nullptr)
	Paging::unmap_page(reinterpret_cast<VirtAddr>(nullptr));
	for (size_t i = 1; i < 512; i++) {
		auto addr = i * Paging::PAGE_SIZE;
		Paging::map_page(addr, addr); // TODO handle failure (unlikely)
	}
	Paging::flush(reinterpret_cast<VirtAddr>(nullptr));

	Debug::log_ok("Memory initialized");
}

void *Memory::allocate(size_t size, size_t allignment, bool clear) {
	void *ptr = heap_ptr;
	heap_ptr += size;

	if (heap_ptr > heap + KERNEL_HEAP_SIZE) {
		Debug::log_failure("Insufficient kernel heap memory");
		heap_ptr -= size;
		return nullptr;
	}

	if (clear) {
		memset(ptr, 0, size);
	}

	if (allignment) {
		// TODO Implement this
		Debug::log_warning("Memory::allocate() with alignment is not yet implemented");
	}

	return ptr;
}

void Memory::deallocate(void *ptr, UNUSED size_t size, UNUSED size_t alignment) {
	if (!ptr) {
		return;
	}

	// TODO Implement this
	Debug::log_warning("Memory::deallocate() is not yet implemented");
}