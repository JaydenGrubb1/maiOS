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
#include <kernel/arch/x86_64/io.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>
#include <lib/stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void __attribute__((interrupt)) keyboard_isr(__attribute__((unused)) Interrupts::StackFrame *frame) {
	char scan_code = IO::in8(0x60);
	char letter = '\0';

	switch (scan_code) {
		case 0x1e:
			letter = 'a';
			break;
		case 0x30:
			letter = 'b';
			break;
		case 0x2e:
			letter = 'c';
			break;
		case 0x20:
			letter = 'd';
			break;
		case 0x12:
			letter = 'e';
			break;
		case 0x21:
			letter = 'f';
			break;
		case 0x22:
			letter = 'g';
			break;
		case 0x23:
			letter = 'h';
			break;
		case 0x17:
			letter = 'i';
			break;
		case 0x24:
			letter = 'j';
			break;
		case 0x25:
			letter = 'k';
			break;
		case 0x26:
			letter = 'l';
			break;
		case 0x32:
			letter = 'm';
			break;
		case 0x31:
			letter = 'n';
			break;
		case 0x18:
			letter = 'o';
			break;
		case 0x19:
			letter = 'p';
			break;
		case 0x10:
			letter = 'q';
			break;
		case 0x13:
			letter = 'r';
			break;
		case 0x1f:
			letter = 's';
			break;
		case 0x14:
			letter = 't';
			break;
		case 0x16:
			letter = 'u';
			break;
		case 0x2f:
			letter = 'v';
			break;
		case 0x11:
			letter = 'w';
			break;
		case 0x2d:
			letter = 'x';
			break;
		case 0x15:
			letter = 'y';
			break;
		case 0x2c:
			letter = 'z';
			break;
			// space key
		case 0x39:
			letter = ' ';
			break;
			// enter key
		case 0x1c:
			letter = '\n';
			break;
			// tab key
		case 0x0f:
			letter = '\t';
			break;
			// period key
		case 0x34:
			letter = '.';
			break;
			// comma key
		case 0x33:
			letter = ',';
			break;
			// hyphen key
		case 0x0c:
			letter = '-';
			break;
			// equals key
		case 0x0d:
			letter = '=';
			break;

		default:
			break;
	}

	putchar(letter);

	PIC::eoi(0x21);
}

#include <lib/stdio.h>

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, void *addr) {
	printf("\033[2J");
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

	IO::out8(0x21, 0b11111101);
	Interrupts::set_isr(0x21, keyboard_isr);

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
	Debug::log_ok("Memory dump complete");

	Debug::log_warning("Entering idle loop...");
	// asm volatile("ud2");
	while (true) {
		// spin-lock
	}

	// this should never be reached
	CPU::halt();
}