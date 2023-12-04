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
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

#define KERNEL_HEAP_SIZE (64 * MiB)

static SECTION(".heap") uint8_t heap[KERNEL_HEAP_SIZE];
static uint8_t *heap_ptr = heap;

static ALIGNED(4 * KiB) uint8_t page_pool[4 * MiB];
static uint8_t *page_pool_ptr = page_pool;

typedef uint64_t PhysAddr;
typedef uint64_t VirtAddr;

struct PageTableEntry {
	uint64_t _data;

	bool is_present() const {
		return _data & 0x1;
	}

	PageTableEntry &set_present(bool present) {
		if (present) {
			_data |= 0x1;
		} else {
			_data &= ~0x1;
		}
		return *this;
	}

	bool is_writable() const {
		return _data & 0x2;
	}

	PageTableEntry &set_writable(bool writable) {
		if (writable) {
			_data |= 0x2;
		} else {
			_data &= ~0x2;
		}
		return *this;
	}

	bool is_huge() const {
		return _data & 0x80;
	}

	PageTableEntry &set_huge(bool huge) {
		if (huge) {
			_data |= 0x80;
		} else {
			_data &= ~0x80;
		}
		return *this;
	}

	uint64_t addr() const {
		return _data & 0xffffffffff000;
	}
};

static inline void flush_tlb(uint64_t addr) {
	asm volatile("invlpg [%0]" ::"r"(addr) : "memory");
}

PhysAddr alloc_page(void) {
	auto ptr = page_pool_ptr;
	page_pool_ptr += 4 * KiB;
	return reinterpret_cast<PhysAddr>(ptr);
}

void map(PhysAddr phys, VirtAddr virt) {
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

	if (!l4_addr[l4_idx].is_present()) {
		auto page = alloc_page();
		l4_addr[l4_idx] = PageTableEntry{page | 0b11}; // Present and writable
	}

	if (!l3_addr[l3_idx].is_present()) {
		auto page = alloc_page();
		l3_addr[l3_idx] = PageTableEntry{page | 0b11}; // Present and writable
	}

	if (!l2_addr[l2_idx].is_present()) { // TODO Check if this is a huge page
		auto page = alloc_page();
		l2_addr[l2_idx] = PageTableEntry{page | 0b11}; // Present and writable
	}

	if (l1_addr[l1_idx].is_present()) {
		Debug::log_failure("Page already mapped");
	}

	l1_addr[l1_idx] = PageTableEntry{phys | 0b11};

	// VERIFY when to flush TLB
	flush_tlb(virt);
}

kstd::optional<PhysAddr> virt2phys(VirtAddr virt) {
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

	if (!(l4_addr[l4_idx].is_present())) {
		return {};
	}

	if (!(l3_addr[l3_idx].is_present())) {
		return {};
	}

	if (!(l2_addr[l2_idx].is_present())) {
		return {};
	}

	if (l2_addr[l2_idx].is_huge()) {
		return l2_addr[l2_idx].addr() | (virt & 0x1fffff);
	}

	if (!(l1_addr[l1_idx].is_present())) {
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

	VirtAddr addr_virt = 0xdeadbeef;
	auto page = alloc_page();
	map(page, addr_virt);
	Debug::log_info("Mapped virtual address: %p => physical address: %p",
					reinterpret_cast<void *>(addr_virt),
					reinterpret_cast<void *>(page));

	// this would be a page fault if it wasn't mapped
	*reinterpret_cast<int *>(addr_virt) = 42;

	// retrieve the physical address of the page
	// should be approximately the same as was returned by alloc_page()
	PhysAddr addr_phys = virt2phys(addr_virt).value_or(0);

	// this read only works because it's also identity mapped
	auto test_val = *reinterpret_cast<int *>(addr_phys);

	Debug::log_info("Value at physical address: %p => %d",
					reinterpret_cast<void *>(addr_phys),
					test_val);

	assert(test_val == 42);
	assert(addr_phys != addr_virt);
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