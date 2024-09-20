/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-05
 * @brief Represents a page table entry
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

#include <kernel/arch/x86_64/memory/physaddr.h>

namespace Memory::Paging {
	struct PageTableEntry {
		/**
		 * @brief The value of the page table entry
		 *
		 */
		uint64_t value;

		/**
		 * @brief Check if the page table entry is present
		 *
		 * @return true if the page table entry is present
		 */
		bool is_present(void) const;

		/**
		 * @brief Set the present bit of the page table entry
		 *
		 * @param present true to set the present bit, false to clear it
		 */
		void set_present(bool present);

		/**
		 * @brief Check if the page table entry is writable
		 *
		 * @return true if the page table entry is writable
		 */
		bool is_writable(void) const;

		/**
		 * @brief Check if the page table entry is huge
		 *
		 * @return true if the page table entry is huge
		 */
		bool is_huge(void) const;

		/**
		 * @brief The physical address of the page frame
		 *
		 * @return
		 */
		PhysAddr page_frame(void) const;
	};
}