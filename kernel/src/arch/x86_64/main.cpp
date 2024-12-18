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

#include <cstddef>
#include <cstdint>
#include <cxxabi.h>

#include <cassert>
#include <functional>
#include <new>
#include <span>

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
#include <kernel/arch/x86_64/tss.h>
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

		Debug::log_warning("Entering idle loop");
		while (true) {
			Scheduler::yield();
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

		char cpu_vendor[13];
		asm volatile("cpuid"
					 : "=b"(reinterpret_cast<uint32_t &>(cpu_vendor[0])),
					   "=c"(reinterpret_cast<uint32_t &>(cpu_vendor[8])),
					   "=d"(reinterpret_cast<uint32_t &>(cpu_vendor[4]))
					 : "a"(0x00000000));
		cpu_vendor[12] = '\0';

		char cpu_brand[49];
		asm volatile("cpuid"
					 : "=a"(reinterpret_cast<uint32_t &>(cpu_brand[0])),
					   "=b"(reinterpret_cast<uint32_t &>(cpu_brand[4])),
					   "=c"(reinterpret_cast<uint32_t &>(cpu_brand[8])),
					   "=d"(reinterpret_cast<uint32_t &>(cpu_brand[12]))
					 : "a"(0x80000002));
		asm volatile("cpuid"
					 : "=a"(reinterpret_cast<uint32_t &>(cpu_brand[16])),
					   "=b"(reinterpret_cast<uint32_t &>(cpu_brand[20])),
					   "=c"(reinterpret_cast<uint32_t &>(cpu_brand[24])),
					   "=d"(reinterpret_cast<uint32_t &>(cpu_brand[28]))
					 : "a"(0x80000003));
		asm volatile("cpuid"
					 : "=a"(reinterpret_cast<uint32_t &>(cpu_brand[32])),
					   "=b"(reinterpret_cast<uint32_t &>(cpu_brand[36])),
					   "=c"(reinterpret_cast<uint32_t &>(cpu_brand[40])),
					   "=d"(reinterpret_cast<uint32_t &>(cpu_brand[44]))
					 : "a"(0x80000004));
		cpu_brand[48] = '\0';

		Debug::log_info("Booted via: %s", bootloader_name);
		Debug::log_info("GRUB options: %s", boot_cmd_line);
		Debug::log_info("CPU: %s (%s)", cpu_brand, cpu_vendor);

		Interrupts::init();
		TSS::init();
		KSyms::init();
		PIC::init();
		Memory::init();

		Debug::log("Initializing global constructors...");
		const std::span ctors(&__kernel_ctors_start, &__kernel_ctors_end);
		for (auto ctor : ctors) {
			std::invoke(ctor);
		}
		Debug::log_ok("Initialized %zu global constructors", ctors.size());

		Time::RTC::init();

		// x86_64 requires SSE and SSE2
		assert(CPU::has_feature(CPU::Feature::SSE));
		assert(CPU::has_feature(CPU::Feature::SSE2));

		Debug::log("Enabling SSE...");
		asm volatile("mov rax, cr0;"
					 "and ax, 0xfffb;"
					 "or ax, 0x2;"
					 "mov cr0, rax;"
					 "mov rax, cr4;"
					 "or ax, 0x600;"
					 "mov cr4, rax" ::: "rax");
		Debug::log_ok("SSE enabled");

		Scheduler::init();
		Scheduler::create_thread(late_init);
		Scheduler::start();
	}
}

KERNEL_ENTRY(Kernel::main);