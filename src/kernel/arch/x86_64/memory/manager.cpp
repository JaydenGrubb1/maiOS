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

#include <kernel/arch/x86_64/memory/manager.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

#define KERNEL_HEAP_SIZE (64 * MiB)

static SECTION(".heap") uint8_t heap[KERNEL_HEAP_SIZE];
static uint8_t *heap_ptr = heap;

struct PageTableEntry {
	uint64_t _data;

	bool present() const {
		return _data & 0x1;
	}

	bool huge() const {
		return _data & 0x80;
	}

	uint64_t addr() const {
		return _data & 0xffffffffff000;
	}
};

kstd::optional<uintptr_t> virt2phys(uintptr_t virt) {
	constexpr uintptr_t recurs = 0x1ff;
	constexpr uintptr_t ext = 0xffffUL << 48;

	uintptr_t l4_idx = (virt >> 39) & 0x1ff;
	uintptr_t l3_idx = (virt >> 30) & 0x1ff;
	uintptr_t l2_idx = (virt >> 21) & 0x1ff;
	uintptr_t l1_idx = (virt >> 12) & 0x1ff;

	auto l4_addr = reinterpret_cast<PageTableEntry *>(ext | (recurs << 39) | (recurs << 30) | (recurs << 21) | (recurs << 12));
	auto l3_addr = reinterpret_cast<PageTableEntry *>(ext | (recurs << 39) | (recurs << 30) | (recurs << 21) | (l4_idx << 12));
	auto l2_addr = reinterpret_cast<PageTableEntry *>(ext | (recurs << 39) | (recurs << 30) | (l4_idx << 21) | (l3_idx << 12));
	auto l1_addr = reinterpret_cast<PageTableEntry *>(ext | (recurs << 39) | (l4_idx << 30) | (l3_idx << 21) | (l2_idx << 12));

	if (!(l4_addr[l4_idx].present())) {
		return {};
	}

	if (!(l3_addr[l3_idx].present())) {
		return {};
	}

	if (!(l2_addr[l2_idx].present())) {
		return {};
	}

	if (l2_addr[l2_idx].huge()) {
		return l2_addr[l2_idx].addr() | (virt & 0x1fffff);
	}

	if (!(l1_addr[l1_idx].present())) {
		return {};
	}

	return l1_addr[l1_idx].addr() | (virt & 0xfff);
}

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

	auto virt = 0xfffffffffffff000;
	auto phys = virt2phys(virt);

	if (phys) {
		Debug::log_info("Virtual address: %p => physical address: %p",
						reinterpret_cast<void *>(virt),
						reinterpret_cast<void *>(*phys));
	} else {
		Debug::log_info("Virtual address: %p => physical address: N/A",
						reinterpret_cast<void *>(virt));
	}

	Debug::log_warning("Memory manager is not yet implemented");
}

void *Memory::allocate(size_t size) {
	void *ptr = heap_ptr;
	heap_ptr += size;

	if (heap_ptr > heap + KERNEL_HEAP_SIZE) {
		Debug::log_failure("Insufficient kernel heap memory");
		heap_ptr -= size;
		return nullptr;
	}

	return ptr;
}

void Memory::deallocate(void *ptr, UNUSED size_t size) {
	if (!ptr) {
		return;
	}

	Debug::log_warning("Memory::deallocate() is not yet implemented");
}