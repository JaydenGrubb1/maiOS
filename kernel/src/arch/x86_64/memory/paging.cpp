/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-05
 * @brief Manages the system's page tables
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <arch/x86_64/memory/frame_allocator.h>
#include <arch/x86_64/memory/page_table.h>
#include <arch/x86_64/memory/paging.h>
#include <debug.h>

using namespace Memory;

constinit const auto l4_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7fbfdfe000);
constinit const auto l3_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7fbfc00000);
constinit const auto l2_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7f80000000);
constinit const auto l1_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff0000000000);

kstd::optional<PhysAddr> Paging::translate(VirtAddr virt) {
	uintptr_t l4_idx = (virt >> 39) & 0x1ffUL;
	uintptr_t l3_idx = (virt >> 30) & 0x3ffffUL;
	uintptr_t l2_idx = (virt >> 21) & 0x7ffffffUL;
	uintptr_t l1_idx = (virt >> 12) & 0xfffffffffUL;

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

bool Paging::map_page(PhysAddr phys, VirtAddr virt, uint64_t flags) {
	uintptr_t l4_idx = (virt >> 39) & 0x1ffUL;
	uintptr_t l3_idx = (virt >> 30) & 0x3ffffUL;
	uintptr_t l2_idx = (virt >> 21) & 0x7ffffffUL;
	uintptr_t l1_idx = (virt >> 12) & 0xfffffffffUL;

	if (!l4_addr[l4_idx].is_present()) {
		auto page = FrameAllocator::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}

		l4_addr[l4_idx] = PageTableEntry{page.value() | 0b11}; // Present and writable
	}

	if (!l3_addr[l3_idx].is_present()) {
		auto page = FrameAllocator::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}

		l3_addr[l3_idx] = PageTableEntry{page.value() | 0b11}; // Present and writable
	}

	if (!l2_addr[l2_idx].is_present()) {
		auto page = FrameAllocator::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}

		l2_addr[l2_idx] = PageTableEntry{page.value() | 0b11}; // Present and writable
	} else if (l2_addr[l2_idx].is_huge()) {
		Debug::log_failure("L2 page already mapped as huge page");
		return false;
	}

	if (l1_addr[l1_idx].is_present()) {
		Debug::log_failure("Page already mapped");
		return false;
	}

	l1_addr[l1_idx] = PageTableEntry{phys | 0b11 | flags}; // Present and writable + flags
	return true;
}

void Paging::unmap_page(VirtAddr virt) {
	uintptr_t l4_idx = (virt >> 39) & 0x1ffUL;
	uintptr_t l3_idx = (virt >> 30) & 0x3ffffUL;
	uintptr_t l2_idx = (virt >> 21) & 0x7ffffffUL;
	uintptr_t l1_idx = (virt >> 12) & 0xfffffffffUL;

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
		l2_addr[l2_idx].set_present(false);
		// flush(virt); // TODO see below
		return;
	}

	if (!l1_addr[l1_idx].is_present()) {
		Debug::log_warning("L1 page already not mapped");
		return;
	}

	l1_addr[l1_idx].set_present(false);
	// flush(virt);
	// TODO add flag to flush or not
	// might want to flush after performing a bunch of unmappings
}