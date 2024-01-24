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

#include <algorithm>
#include <bitfield>

#include <kernel/arch/x86_64/memory/frame_allocator.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/regions.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>

using namespace Memory;

extern char __kernel_end;

static kstd::vector<kstd::vector<kstd::bitfield<FrameAllocator::Zone>>> page_bitmaps;
static kstd::vector<size_t> allocated_pages;
static size_t total_memory = 0;

void FrameAllocator::init() {
	Debug::log("Initializing frame allocator...");

	assert(!Memory::regions().empty());

	auto kernel_end = Paging::translate(reinterpret_cast<VirtAddr>(&__kernel_end));
	if (!kernel_end.has_value()) {
		Debug::log_failure("Could not translate kernel end");
		return;
	}

	// TODO change this to a InitRD::addr() function or something
	auto module_end = reinterpret_cast<Multiboot2::ModuleInfo const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::MODULES))->mod_end;
	auto final_page = Paging::round_up(kstd::max(kernel_end.value(), static_cast<PhysAddr>(module_end)));

	page_bitmaps.reserve(Memory::regions().size());
	allocated_pages.reserve(Memory::regions().size());

	for (auto &region : Memory::regions()) {
		total_memory += region.size();

		page_bitmaps.emplace_back();
		allocated_pages.emplace_back(0);

		if (final_page >= region.upper) {
			page_bitmaps.back().resize(region.zones(), ~0ULL);
			allocated_pages.back() = region.pages();
		} else if (region.contains(final_page)) {
			allocated_pages.back() = (final_page - region.lower) / Paging::PAGE_SIZE;
			auto zones = allocated_pages.back() / FrameAllocator::ZONE_SIZE;
			auto bits = allocated_pages.back() % FrameAllocator::ZONE_SIZE;

			page_bitmaps.back().reserve(zones + 1);
			page_bitmaps.back().resize(zones, ~0ULL);
			if (bits != 0) {
				page_bitmaps.back().emplace_back(bits, true);
			}
		}
	}

	Debug::log_info("Total memory: %lu MiB", total_memory / MiB);
	Debug::log_ok("Frame allocator initialized");
}

kstd::optional<PhysAddr> FrameAllocator::alloc(void) {
	for (size_t i = 0; i < Memory::regions().size(); i++) {
		auto &region = Memory::regions()[i];
		auto &bitmap = page_bitmaps[i];
		auto &allocated = allocated_pages[i];

		if (allocated == region.pages()) {
			continue;
		}

		for (size_t zone = 0; zone < bitmap.size(); zone++) {
			if (bitmap[zone].full()) {
				continue;
			}

			for (size_t bit = 0; bit < ZONE_SIZE; bit++) {
				if (!bitmap[zone][bit]) {
					bitmap[zone].set(bit, true);
					allocated++;

					auto addr = region.lower + (zone * ZONE_SIZE + bit) * Paging::PAGE_SIZE;
#ifdef DEBUG
					assert(region.contains(addr));
#endif
					return addr;
				}
			}
		}

		bitmap.emplace_back(1);
		allocated++;
		auto addr = region.lower + (bitmap.size() - 1) * ZONE_SIZE * Paging::PAGE_SIZE;
#ifdef DEBUG
		assert(region.contains(addr));
#endif
		return addr;
	}

	return kstd::nullopt;
}

void FrameAllocator::free(PhysAddr addr) {
	size_t idx = 0;
	for (auto &region : Memory::regions()) {
		if (!region.contains(addr)) {
			idx++;
			continue;
		}

		auto zone = (addr - region.lower) / (ZONE_SIZE * Paging::PAGE_SIZE);
		auto bit = (addr - region.lower) / Paging::PAGE_SIZE % ZONE_SIZE;

		page_bitmaps[idx][zone].set(bit, false);
		allocated_pages[idx]--;
		return;
	}

	Debug::log_warning("FrameAllocator::free() called with invalid address");
}