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
	 * @brief Flags for a page table entry
	 *
	 */
	enum class Flags : uint64_t {
		NONE = 0,							   // No flags
		PRESENT = 1UL << 0,					   // Page is present in memory
		WRITABLE = 1UL << 1,				   // Page is writable
		USER = 1UL << 2,					   // Page is accessible by user mode
		WRITE_THROUGH = 1UL << 3,			   // Page writes are written through to memory
		CACHE_DISABLE = 1UL << 4,			   // Page is not cached
		PAT = 1UL << 7,						   // Page uses PAT (Page Attribute Table)
		GLOBAL = 1UL << 8,					   // Page is global
		NO_EXECUTE = 1UL << 63,				   // Page cannot be executed
		WRITE_COMBINING = PAT | WRITE_THROUGH, // Page is write-combining
		WRITE_PROTECTED = PAT | CACHE_DISABLE, // Page is write-protected
	};

	/**
	 * @brief Bitwise OR operator for Flags
	 *
	 * @param lhs The left-hand side of the operator
	 * @param rhs The right-hand side of the operator
	 * @return The result of the bitwise OR
	 */
	inline constexpr Flags operator|(Flags lhs, Flags rhs) {
		return static_cast<Flags>(static_cast<uint64_t>(lhs) | static_cast<uint64_t>(rhs));
	}

	/**
	 * @brief Initializes the paging system
	 *
	 */
	void init(void);

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
	 *
	 * @note Flags::PRESENT is always set
	 */
	bool map_page(PhysAddr phys, VirtAddr virt, Flags flags = Flags::NONE);

	/**
	 * @brief Unmaps a virtual address
	 *
	 * @param virt The virtual address to unmap
	 * @param auto_flush Whether to flush the TLB after unmapping
	 */
	void unmap_page(VirtAddr virt, bool auto_flush = true);

	/**
	 * @brief Translates a virtual address to a physical address
	 *
	 * @param virt The virtual address to translate
	 * @return The physical address, or nullopt if the address is not mapped
	 */
	std::optional<PhysAddr> translate(VirtAddr virt);

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