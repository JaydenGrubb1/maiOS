/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-16
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <lib/libc++/optional.h>
#include <lib/libc++/vector.h>

#include <kernel/arch/x86_64/memory/memory_region.h>

namespace Memory::FrameAllocator {
	void init(const kstd::vector<MemoryRegion> &memory_regions);
	kstd::optional<PhysAddr> alloc(void);
	void free(PhysAddr);
}