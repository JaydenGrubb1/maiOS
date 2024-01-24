/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cassert>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts/apic.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/memory/physaddr.h>
#include <kernel/debug.h>

#define APIC_BASE_ADDR 0xfffff000
#define APIC_BASE_ENABLE 0x800
#define APIC_BASE_BSP 0x100

static volatile uint32_t *apic_addr = nullptr;

void APIC::init(void) {
	Debug::log("Initializing Local APIC...");

	uint64_t apic_base = CPU::get_msr(IA32_APIC_BASE_MSR);
	assert(apic_base & APIC_BASE_BSP);
	apic_addr = reinterpret_cast<uint32_t *>(apic_base & APIC_BASE_ADDR);
	Debug::log_info("APIC base address: %p", apic_addr);

	Memory::Paging::map_page(apic_base, apic_base);
	// TODO map as strong uncachable
	// TODO don't identity map ???

	write(Register::SVR, 0xff); // VERIFY correct value

	PIC::disable();
	// TODO disable IMCR PIC mode

	apic_base |= APIC_BASE_ENABLE;
	CPU::set_msr(IA32_APIC_BASE_MSR, apic_base);

	Debug::log_ok("Local APIC initialized");
}

uint32_t APIC::read(Register reg) {
	assert(apic_addr != nullptr);
	return apic_addr[static_cast<uint16_t>(reg) / sizeof(uint32_t)];
}

void APIC::write(Register reg, uint32_t value) {
	assert(apic_addr != nullptr);
	apic_addr[static_cast<uint16_t>(reg) / sizeof(uint32_t)] = value;
}

void APIC::eoi(void) {
	write(Register::EOI, 0);
}