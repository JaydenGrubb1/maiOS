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

#include <kernel/arch/x86_64/memory/page_table.h>

using namespace Memory::Paging;

bool PageTableEntry::is_present(void) const {
	return (value & 0x1) == 1;
}

void PageTableEntry::set_present(bool present) {
	if (present) {
		value |= 0x1;
	} else {
		value &= ~0x1;
	}
}

bool PageTableEntry::is_writable(void) const {
	return (value & 0x2) == 2;
}

bool PageTableEntry::is_huge(void) const {
	return (value & 0x80) == 0x80;
}

uint64_t PageTableEntry::page_frame(void) const {
	return value & 0x000ffffffffff000; // VERIFY is this mask correct?
}