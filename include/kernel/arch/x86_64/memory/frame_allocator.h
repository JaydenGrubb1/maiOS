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

#include <kernel/arch/x86_64/memory/physaddr.h>

namespace Memory::FrameAllocator {
	kstd::optional<PhysAddr> alloc(void);
	void free(PhysAddr);
}