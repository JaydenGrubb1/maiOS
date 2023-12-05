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

bool Memory::Paging::PageTableEntry::is_present() const {
	return (value & 0x1) == 1;
}

bool Memory::Paging::PageTableEntry::is_writable() const {
	return (value & 0x2) == 2;
}

bool Memory::Paging::PageTableEntry::is_huge() const {
	return (value & 0x80) == 0x80;
}

uint64_t Memory::Paging::PageTableEntry::page_frame() const {
	return value & 0x000ffffffffff000; // VERIFY is this mask correct?
}