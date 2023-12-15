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

#pragma once

#define __need_size_t
#include <stddef.h>

#include <lib/libc++/optional.h>

#include <kernel/arch/x86_64/memory/physaddr.h>
#include <kernel/arch/x86_64/memory/virtaddr.h>

namespace Memory::Paging {
	constexpr size_t PAGE_SIZE = 4 * KiB;

	inline void flush(VirtAddr virt) {
		asm volatile("invlpg [%0]" ::"r"(virt) : "memory");
	}

	bool map_page(PhysAddr phys, VirtAddr virt, uint64_t flags = 0); // TODO better flags parameter
	void unmap_page(VirtAddr virt);
	kstd::optional<PhysAddr> translate(VirtAddr virt);
}