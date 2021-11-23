/**
 * @file kernel/kmain.cpp
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

#include <multiboot2.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, uint32_t addr)
{
	bool success = Multiboot2::init((uint8_t *)addr);

	auto name = Multiboot2::get<char>(Multiboot2::BOOTLOADER_NAME);
	auto mmap = Multiboot2::get<Multiboot2::MemoryMap>(Multiboot2::MEMORY_MAP)->getEntries();
	auto map_entries = Multiboot2::get<Multiboot2::MemoryMap>(Multiboot2::MEMORY_MAP)->entryCount();

	auto bootServices = Multiboot2::get<bool>(Multiboot2::EFI_BOOT_SERVICES_NOT_TERMINATED) != nullptr;

	auto nll = nullptr;
	auto bootDev = Multiboot2::get<Multiboot2::BiosBootDevice>(Multiboot2::BIOS_BOOT_DEVICE);
	return;
}