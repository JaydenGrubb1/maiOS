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

#include <kernel/arch/ksyms.h>
#include <kernel/arch/version.h>
#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/multiboot2.h>
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

	Debug::log_info("Booted via: %s", bootloader_name);
	Debug::log_info("GRUB options: %s", boot_cmd_line);

	KSyms::init();
	PIC::init();
	Interrupts::init();
	Interrupts::sti();

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

	Debug::log_warning("Entering idle loop...");
	while (true) {
		// spin-lock
	}

	// this should never be reached
	CPU::halt();
}