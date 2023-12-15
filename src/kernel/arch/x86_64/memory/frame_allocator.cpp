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
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/debug.h>

using namespace Memory;

#define PAGE_POOL_SIZE (4 * MiB)

static ALIGNED(Paging::PAGE_SIZE) uint8_t page_pool[PAGE_POOL_SIZE];
static uint8_t *page_pool_ptr = page_pool;

kstd::optional<PhysAddr> FrameAllocator::alloc(void) {
	auto ptr = page_pool_ptr;
	page_pool_ptr += Paging::PAGE_SIZE;

	if (page_pool_ptr > page_pool + PAGE_POOL_SIZE) {
		Debug::log_failure("Out of memory");
		return {};
	}

	return reinterpret_cast<Memory::PhysAddr>(ptr);
}

void FrameAllocator::free(PhysAddr) {
	// TODO implement
}