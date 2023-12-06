/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-05
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/memory/pagetable.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/debug.h>

static ALIGNED(4 * KiB) uint8_t page_pool[4 * MiB];
static uint8_t *page_pool_ptr = page_pool;

Memory::PhysAddr Memory::Paging::alloc_page(void) {
	auto ptr = page_pool_ptr;
	page_pool_ptr += 4 * KiB;
	return reinterpret_cast<Memory::PhysAddr>(ptr);
}
// TODO replace with proper frame allocator

void Memory::Paging::flush(Memory::VirtAddr virt) {
	asm volatile("invlpg [%0]" ::"r"(virt) : "memory");
}

kstd::optional<Memory::PhysAddr> Memory::Paging::translate(Memory::VirtAddr virt) {
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
		return kstd::nullopt;
	}

	if (!(l3_addr[l3_idx].is_present())) {
		return kstd::nullopt;
	}

	if (!(l2_addr[l2_idx].is_present())) {
		return kstd::nullopt;
	}

	if (l2_addr[l2_idx].is_huge()) {
		return l2_addr[l2_idx].page_frame() | (virt & 0x1fffff);
	}

	if (!(l1_addr[l1_idx].is_present())) {
		return kstd::nullopt;
	}

	return l1_addr[l1_idx].page_frame() | (virt & 0xfff);
}

void Memory::Paging::map_page(Memory::PhysAddr phys, Memory::VirtAddr virt) {
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

	if (!l2_addr[l2_idx].is_present()) {
		auto page = alloc_page();
		l2_addr[l2_idx] = PageTableEntry{page | 0b11}; // Present and writable
	} else if (l2_addr[l2_idx].is_huge()) {
		Debug::log_failure("L2 page already mapped as huge page");
		return;
	}

	if (l1_addr[l1_idx].is_present()) {
		Debug::log_failure("Page already mapped");
		return;
	}

	l1_addr[l1_idx] = PageTableEntry{phys | 0b11}; // Present and writable
}

void Memory::Paging::unmap_page(Memory::VirtAddr virt) {
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
		Debug::log_warning("L4 page already not mapped");
		return;
	}

	if (!l3_addr[l3_idx].is_present()) {
		Debug::log_warning("L3 page already not mapped");
		return;
	}

	if (!l2_addr[l2_idx].is_present()) {
		Debug::log_warning("L2 page already not mapped");
		return;
	}

	if (l2_addr[l2_idx].is_huge()) {
		l2_addr[l2_idx] = PageTableEntry{0};
		// flush(virt); // TODO see below
		return;
	}

	if (!l1_addr[l1_idx].is_present()) {
		Debug::log_warning("L1 page already not mapped");
		return;
	}

	l1_addr[l1_idx] = PageTableEntry{0};
	// flush(virt);
	// TODO add flag to flush or not
	// might want to flush after performing a bunch of unmappings
}