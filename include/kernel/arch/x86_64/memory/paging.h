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

#pragma once

#define __need_size_t
#include <stddef.h>

#include <optional>

#include <kernel/arch/x86_64/memory/physaddr.h>
#include <kernel/arch/x86_64/memory/virtaddr.h>
#include <kernel/defines.h>

namespace Memory::Paging {
	/**
	 * @brief The size of a page
	 *
	 */
	constexpr size_t PAGE_SIZE = 4 * KiB;

	/**
	 * @brief Flushes the given virtual address from the Translation Lookaside Buffer (TLB)
	 *
	 * @param virt The virtual address to flush
	 */
	inline void flush(VirtAddr virt) {
		asm volatile("invlpg [%0]" ::"r"(virt) : "memory");
	}

	/**
	 * @brief Flushes the entire Translation Lookaside Buffer (TLB)
	 *
	 */
	inline void flush() {
		asm volatile("mov rax, cr3; mov cr3, rax");
	}

	/**
	 * @brief Converts a physical address to a kernel virtual address
	 *
	 * @param addr The physical address to convert
	 * @return The kernel virtual address
	 */
	constexpr VirtAddr to_kernel(PhysAddr addr) {
		assert(addr < 1 * GiB);
		return reinterpret_cast<VirtAddr>(addr + 0xffffffff80000000);
	}

	/**
	 * @brief Maps a physical page to a virtual address
	 *
	 * @param phys The physical address to map
	 * @param virt The virtual address to map to
	 * @param flags The flags to use for the mapping
	 * @return true if the mapping was successful
	 */
	bool map_page(PhysAddr phys, VirtAddr virt, uint64_t flags = 0); // TODO better flags parameter

	/**
	 * @brief Unmaps a virtual address
	 *
	 * @param virt The virtual address to unmap
	 */
	void unmap_page(VirtAddr virt);

	/**
	 * @brief Translates a virtual address to a physical address
	 *
	 * @param virt The virtual address to translate
	 * @return The physical address, or nullopt if the address is not mapped
	 */
	kstd::optional<PhysAddr> translate(VirtAddr virt);

	/**
	 * @brief Rounds an address down to the nearest page boundary
	 *
	 * @param addr The address to round
	 * @return The rounded address
	 */
	constexpr PhysAddr round_down(PhysAddr addr) {
		return addr & ~(PAGE_SIZE - 1);
	}

	/**
	 * @brief Rounds an address up to the nearest page boundary
	 *
	 * @param addr The address to round
	 * @return The rounded address
	 */
	constexpr PhysAddr round_up(PhysAddr addr) {
		return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	}
}