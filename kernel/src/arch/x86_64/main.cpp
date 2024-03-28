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
#include <cxxabi.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <cassert>
#include <new>
#include <utility>

#include <kernel/arch/framebuffer.h>
#include <kernel/arch/ksyms.h>
#include <kernel/arch/memory.h>
#include <kernel/arch/x86_64/boot/entry.h>
#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/interrupts/pic.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/arch/x86_64/scheduler.h>
#include <kernel/arch/x86_64/time/rtc.h>
#include <kernel/debug.h>
#include <kernel/panic.h>
#include <kernel/version.h>

typedef void (*Constructor)(void);

extern "C" Constructor __kernel_ctors_start;
extern "C" Constructor __kernel_ctors_end;

namespace Kernel {
	/**
	 * @brief Late initialization function
	 *
	 */
	[[noreturn]] void late_init(void) {
		Debug::log("Starting late initialization...");

		Debug::log_warning("Entering idle loop");
		while (true) {
			Scheduler::yield();
			asm volatile("pause");
		}
	}

	/**
	 * @brief Main entry point for the operating (64-bit)
	 *
	 * @param magic The magic number passed by multiboot2
	 * @param addr The address of the multiboot2 info structure
	 */
	[[noreturn]] void main(uint32_t magic, void *addr) {
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

		auto bootloader_name = static_cast<Multiboot2::StringTag const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::BOOTLOADER_NAME))->string;
		auto boot_cmd_line = static_cast<Multiboot2::StringTag const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::BOOT_CMD_LINE))->string;

		uint32_t eax;
		char cpu_vendor[13];
		__get_cpuid(0x00000000,
					&eax,
					reinterpret_cast<uint32_t *>(&cpu_vendor[0]),
					reinterpret_cast<uint32_t *>(&cpu_vendor[8]),
					reinterpret_cast<uint32_t *>(&cpu_vendor[4]));
		cpu_vendor[12] = '\0';

		char cpu_brand[49];
		__get_cpuid(0x80000002,
					reinterpret_cast<uint32_t *>(&cpu_brand[0]),
					reinterpret_cast<uint32_t *>(&cpu_brand[4]),
					reinterpret_cast<uint32_t *>(&cpu_brand[8]),
					reinterpret_cast<uint32_t *>(&cpu_brand[12]));
		__get_cpuid(0x80000003,
					reinterpret_cast<uint32_t *>(&cpu_brand[16]),
					reinterpret_cast<uint32_t *>(&cpu_brand[20]),
					reinterpret_cast<uint32_t *>(&cpu_brand[24]),
					reinterpret_cast<uint32_t *>(&cpu_brand[28]));
		__get_cpuid(0x80000004,
					reinterpret_cast<uint32_t *>(&cpu_brand[32]),
					reinterpret_cast<uint32_t *>(&cpu_brand[36]),
					reinterpret_cast<uint32_t *>(&cpu_brand[40]),
					reinterpret_cast<uint32_t *>(&cpu_brand[44]));
		cpu_brand[48] = '\0';

		Debug::log_info("Booted via: %s", bootloader_name);
		Debug::log_info("GRUB options: %s", boot_cmd_line);
		Debug::log_info("CPU: %s (%s)", cpu_brand, cpu_vendor);

		Interrupts::init();
		KSyms::init();
		PIC::init();
		Memory::init();

		Debug::log("Initializing global constructors...");
		size_t counter = 0;
		for (Constructor *ctor = &__kernel_ctors_start; ctor < &__kernel_ctors_end; ctor++) {
			(*ctor)();
			counter++;
		}
		Debug::log_ok("Initialized %lu global constructors", counter);

		Time::RTC::init();
		Scheduler::init();

		Interrupts::enable();
		Debug::log_ok("Interrupts enabled");

		{
			namespace FB = Graphics::Framebuffer;
			FB::init();

			for (int y = 0; y < FB::height(); y++) {
				uint32_t *pixel = FB::addr() + (y * FB::pitch() / 4);
				for (int x = 0; x < FB::width(); x++) {
					uint8_t r = (x * 255) / FB::width();
					uint8_t g = (y * 255) / FB::height();
					uint8_t b = 0;
					*pixel = 0xff000000 | (r << 16) | (g << 8) | b;
					pixel++;
				}
			}
		}

		Scheduler::create_task(late_init);
		Scheduler::start();

		Kernel::panic("Kernel main returned");
		std::unreachable();
	}
}

KERNEL_ENTRY(Kernel::main);