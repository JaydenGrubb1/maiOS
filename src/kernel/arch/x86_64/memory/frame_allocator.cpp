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

#include <defines.h>

#include <kernel/arch/x86_64/memory/frame_allocator.h>

using namespace Memory;

static ALIGNED(4 * KiB) uint8_t page_pool[4 * MiB];
static uint8_t *page_pool_ptr = page_pool;

PhysAddr FrameAllocator::alloc(void) {
	auto ptr = page_pool_ptr;
	page_pool_ptr += 4 * KiB;
	return reinterpret_cast<Memory::PhysAddr>(ptr);
}

void FrameAllocator::free(PhysAddr) {
	// TODO implement
}