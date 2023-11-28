/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

#define MULTIBOOT2_MAGIC 0x36D76289

namespace Multiboot2 {
	enum class BootInfoType : uint32_t {
		END = 0,
		BOOT_CMD_LINE = 1,
		BOOTLOADER_NAME = 2,
		// MODULES = 3,
		// BASIC_MEMORY_INFO = 4,
		// BIOS_BOOT_DEVICE = 5,
		MEMORY_MAP = 6,
		// VBE_INFO = 7,
		// FRAMEBUFFER_INFO = 8,
		ELF_SYMBOLS = 9,
		// APM_TABLE = 10,
		// EFI_32_SYSTEM_TABLE = 11,
		// EFI_64_SYSTEM_TABLE = 12,
		// SMBIOS_TABLES = 13,
		// ACPI_RSDP_1 = 14,
		// ACPI_RSDP_2 = 15,
		// NETWORKING_INFO = 16,
		// EFI_MEMORY_MAP = 17,
		// EFI_BOOT_SERVICES_NOT_TERMINATED = 18,
		// EFI_32_IMAGE_HANDLE_PTR = 19,
		// EFI_64_IMAGE_HANDLE_PTR = 20,
		// EFI_IMAGE_LOAD_BASE_ADDRESS = 21
	};
	// TODO Enable more types as they are implemented

	struct StringTag {
		uint32_t type;
		uint32_t size;
		char string[0];
	};

	enum class MemoryMapEntryType : uint32_t {
		AVAILABLE = 1,
		RESERVED = 2,
		ACPI_RECLAIMABLE = 3,
		ACPI_NVS = 4,
		BAD = 5
	};

	struct MemoryMapEntry {
		uint64_t base;
		uint64_t length;
		MemoryMapEntryType type;
		unsigned : 32;
	};

	struct MemoryMap {
		uint32_t type = 6;
		uint32_t size;
		uint32_t entry_size;
		uint32_t entry_ver;
		MemoryMapEntry entries[0];
	};

	struct ELFSymbols {
		uint32_t type = 9;
		uint32_t size;
		uint32_t entry_num;
		uint32_t entry_size;
		uint32_t shndx;
		char sections[0];
	};

	void init(uint32_t magic, void *addr);

	[[nodiscard]] void const* get_entry(BootInfoType type);
}