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

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/memory/page_table.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physical_memory.h>
#include <kernel/debug.h>

using namespace Memory;

const auto l4_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7fbfdfe000);
const auto l3_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7fbfc00000);
const auto l2_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff7f80000000);
const auto l1_addr = reinterpret_cast<Paging::PageTableEntry *>(0xffffff0000000000);

void Paging::init(void) {
	Debug::log("Initializing paging...");

	Debug::log("Configuring PAT...");
	uint64_t msr = CPU::get_msr(IA32_PAT_MSR);

	// PAT[0] = Writeback (default)
	// PAT[1] = Write-Through (default)
	// PAT[2] = Uncached (default)
	// PAT[3] = Uncacheable (default)
	// PAT[4] = Writeback (default)

	// PAT[5] = Write-Combining
	msr &= ~(0xffUL << 40);
	msr |= (0x1UL << 40);
	Debug::log_info("PAT[5] = Write-Combining");

	// PAT[6] = Write-Protected
	msr &= ~(0xffUL << 48);
	msr |= (0x5UL << 48);
	Debug::log_info("PAT[6] = Write-Protected");

	// PAT[7] = Uncacheable (default)

	CPU::set_msr(IA32_PAT_MSR, msr);

	Debug::log_ok("Paging initialized");
}

std::optional<PhysAddr> Paging::translate(VirtAddr virt) {
	uintptr_t l4_idx = (virt >> 39) & 0x1ffUL;
	uintptr_t l3_idx = (virt >> 30) & 0x3ffffUL;
	uintptr_t l2_idx = (virt >> 21) & 0x7ffffffUL;
	uintptr_t l1_idx = (virt >> 12) & 0xfffffffffUL;

	if (!(l4_addr[l4_idx].is_present())) {
		return std::nullopt;
	}

	if (!(l3_addr[l3_idx].is_present())) {
		return std::nullopt;
	}

	if (!(l2_addr[l2_idx].is_present())) {
		return std::nullopt;
	}

	if (l2_addr[l2_idx].is_huge()) {
		return l2_addr[l2_idx].page_frame() | (virt & 0x1fffff);
	}

	if (!(l1_addr[l1_idx].is_present())) {
		return std::nullopt;
	}

	return l1_addr[l1_idx].page_frame() | (virt & 0xfff);
}

bool Paging::map_page(PhysAddr phys, VirtAddr virt, Flags flags) {
	uintptr_t l4_idx = (virt >> 39) & 0x1ffUL;
	uintptr_t l3_idx = (virt >> 30) & 0x3ffffUL;
	uintptr_t l2_idx = (virt >> 21) & 0x7ffffffUL;
	uintptr_t l1_idx = (virt >> 12) & 0xfffffffffUL;

	if (!l4_addr[l4_idx].is_present()) {
		auto page = PhysicalMemory::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}
		l4_addr[l4_idx] = PageTableEntry{page.value() | static_cast<uint64_t>(Flags::PRESENT | Flags::WRITABLE)};
	}

	if (!l3_addr[l3_idx].is_present()) {
		auto page = PhysicalMemory::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}
		l3_addr[l3_idx] = PageTableEntry{page.value() | static_cast<uint64_t>(Flags::PRESENT | Flags::WRITABLE)};
	}

	if (!l2_addr[l2_idx].is_present()) {
		auto page = PhysicalMemory::alloc();
		if (!page.has_value()) {
			Debug::log_failure("Failed to allocate page");
			return false;
		}
		l2_addr[l2_idx] = PageTableEntry{page.value() | static_cast<uint64_t>(Flags::PRESENT | Flags::WRITABLE)};
	} else if (l2_addr[l2_idx].is_huge()) {
		Debug::log_failure("L2 page already mapped as huge page");
		return false;
	}

	if (l1_addr[l1_idx].is_present()) {
		Debug::log_failure("Page already mapped");
		return false;
	}

	l1_addr[l1_idx] = PageTableEntry{phys | static_cast<uint64_t>(flags | Flags::PRESENT)};
	return true;
}

void Paging::unmap_page(VirtAddr virt, bool auto_flush) {
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
		if (auto_flush) {
			flush(virt);
		}
		return;
	}

	if (!l1_addr[l1_idx].is_present()) {
		Debug::log_warning("L1 page already not mapped");
		return;
	}

	l1_addr[l1_idx].set_present(false);
	if (auto_flush) {
		flush(virt);
	}
}