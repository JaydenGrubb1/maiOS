/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-16
 * @brief Handles the allocation of physical page frames
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

static size_t total_memory = 0;

void FrameAllocator::init(const kstd::vector<MemoryRegion> &memory_regions) {
	Debug::log("Initializing frame allocator...");

	for (auto &region : memory_regions) {
		total_memory += region.length;

		// TODO do something with this memory
	}

	Debug::log_info("Total memory: %lu MiB", total_memory / MiB);
	Debug::log_ok("Frame allocator initialized");
}

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