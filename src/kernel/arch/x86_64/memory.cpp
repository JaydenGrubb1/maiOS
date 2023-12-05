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
#include <lib/libc/string.h>

#include <kernel/arch/x86_64/memory.h>
#include <kernel/arch/x86_64/memory/pagetable.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physaddr.h>
#include <kernel/arch/x86_64/memory/virtaddr.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

#define KERNEL_HEAP_SIZE (64 * MiB)

static SECTION(".heap") uint8_t heap[KERNEL_HEAP_SIZE];
static uint8_t *heap_ptr = heap;

void Memory::init(void) {
	// TODO Actually implement memory management

	Debug::log_info("Multiboot2 provided physical memory map:");
	auto mmap = reinterpret_cast<Multiboot2::MemoryMap const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::MEMORY_MAP));
	for (size_t i = 0; i < (mmap->size - 16) / mmap->entry_size; i++) {
		auto mem = mmap->entries[i];
		Debug::log("- [mem %#.16lx-%#.16lx] %s",
				   mem.base,
				   mem.base + mem.length,
				   mem.type == Multiboot2::MemoryMapEntryType::AVAILABLE ? "available" : "reserved");
		// TODO Add more types
	}

	VirtAddr virt = 0xdeadbeef;
	auto page = Paging::alloc_page();
	Paging::map_page(page, virt);

	PhysAddr phys = Paging::translate(virt).value_or(0);
	Debug::log_test("Virtual address: %p => physical address: %p",
					reinterpret_cast<void *>(virt),
					reinterpret_cast<void *>(phys));
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