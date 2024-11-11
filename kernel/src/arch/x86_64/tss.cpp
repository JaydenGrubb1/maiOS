/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-11-11
 * @brief Manages the Task State Segment (TSS)
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cstdint>
#include <cstring>

#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/tss.h>
#include <kernel/debug.h>
#include <kernel/defines.h>

struct TSSEntry {
	unsigned : 32;
	uint64_t rsp[3];
	long : 64;
	uint64_t ist[7];
	long : 64;
	unsigned : 16;
	uint16_t iomap_base;
} PACKED;

struct TSSDescriptor {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t type : 4;
	uint8_t flags_low : 4;
	uint8_t limit_high : 4;
	uint8_t flags_high : 4;
	uint8_t base_high;
	uint32_t base_upper;
	unsigned : 32;
} PACKED;

static TSSEntry tss;
extern char gdt[];

void TSS::init(void) {
	Debug::log("Initializing TSS...");

	Debug::log("Configuring TSS...");
	memset(&tss, 0, sizeof(tss));
	tss.rsp[0] = reinterpret_cast<uint64_t>(__builtin_frame_address(0));
	tss.iomap_base = sizeof(TSSEntry);

	Debug::log("Configuring TSS descriptor...");
	TSSDescriptor *tss_descriptor = reinterpret_cast<TSSDescriptor *>(&gdt[GDT_TSS]);
	memset(tss_descriptor, 0, sizeof(TSSDescriptor));

	tss_descriptor->limit_low = sizeof(tss) - 1;
	tss_descriptor->limit_high = 0;
	tss_descriptor->base_low = reinterpret_cast<uintptr_t>(&tss) & 0xFFFF;
	tss_descriptor->base_mid = (reinterpret_cast<uintptr_t>(&tss) >> 16) & 0xFF;
	tss_descriptor->base_high = (reinterpret_cast<uintptr_t>(&tss) >> 24) & 0xFF;
	tss_descriptor->base_upper = (reinterpret_cast<uintptr_t>(&tss) >> 32) & 0xFFFFFFFF;
	tss_descriptor->type = 0x9;		  // TSS (available) type
	tss_descriptor->flags_low = 0x8;  // Present, DPL 0
	tss_descriptor->flags_high = 0x2; // Long mode

	Debug::log("Loading TSS...");
	asm volatile("ltr %0" ::"r"(GDT_TSS));

	Debug::log_ok("TSS initialized");
}