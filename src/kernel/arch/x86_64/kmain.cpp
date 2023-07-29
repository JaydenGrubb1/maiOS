/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-20
 * @brief Main entry point for the operating system (64-bit)
 *
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cpuid.h>
#include <kernel/arch/ksyms.h>
#include <kernel/arch/version.h>
#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/cxxabi.h>
#include <kernel/debug.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, void *addr) {
	Debug::log("Booting %s v%d.%d.%d (%s) %s #%s %s",
			   __kernel_name,
			   __kernel_version_major,
			   __kernel_version_minor,
			   __kernel_version_patch,
			   __kernel_arch,
			   __kernel_compiler,
			   __kernel_build_date,
			   __kernel_build_time);

	Multiboot2::init(magic, addr);

	auto bootloader_name = ((Multiboot2::StringTag *)Multiboot2::get_entry(Multiboot2::BOOTLOADER_NAME))->string;
	auto boot_cmd_line = ((Multiboot2::StringTag *)Multiboot2::get_entry(Multiboot2::BOOT_CMD_LINE))->string;

	char cpu_vendor[13];
	__get_cpuid(0x00000000, nullptr, (uint32_t *)&cpu_vendor[0],
				(uint32_t *)&cpu_vendor[8], (uint32_t *)&cpu_vendor[4]);
	cpu_vendor[12] = '\0';

	char cpu_brand[49];
	__get_cpuid(0x80000002, (uint32_t *)&cpu_brand[0], (uint32_t *)&cpu_brand[4],
				(uint32_t *)&cpu_brand[8], (uint32_t *)&cpu_brand[12]);
	__get_cpuid(0x80000003, (uint32_t *)&cpu_brand[16], (uint32_t *)&cpu_brand[20],
				(uint32_t *)&cpu_brand[24], (uint32_t *)&cpu_brand[28]);
	__get_cpuid(0x80000004, (uint32_t *)&cpu_brand[32], (uint32_t *)&cpu_brand[36],
				(uint32_t *)&cpu_brand[40], (uint32_t *)&cpu_brand[44]);
	cpu_brand[48] = '\0';

	Debug::log_info("Booted via: %s", bootloader_name);
	Debug::log_info("GRUB options: %s", boot_cmd_line);
	Debug::log_info("CPU: %s (%s)", cpu_brand, cpu_vendor);

	Interrupts::init();
	KSyms::init();
	PIC::init();

	// TODO Implement memory management
	Debug::log_info("Multiboot2 provided physical memory map:");
	auto mmap = (Multiboot2::MemoryMap *)Multiboot2::get_entry(Multiboot2::MEMORY_MAP);
	for (size_t i = 0; i < (mmap->size - 16) / mmap->entry_size; i++) {
		auto mem = mmap->entries[i];
		Debug::log("- [mem %#.16lx-%#.16lx] %s",
				   mem.base,
				   mem.base + mem.length,
				   mem.type == Multiboot2::MemoryMapEntryType::AVAILABLE ? "available" : "reserved");
		// TODO Add more types
	}

	Interrupts::enable();
	Debug::log_ok("Interrupts enabled");

	Debug::log_warning("Entering idle loop...");
	while (true) {
		// spin-lock
		asm volatile("hlt");
	}

	// this should never be reached
	CPU::halt();
}