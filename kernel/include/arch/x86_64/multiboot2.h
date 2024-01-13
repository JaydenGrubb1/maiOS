/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief Various structures and functions for reading multiboot2 boot info
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
	/**
	 * @brief Multiboot2 boot info type
	 *
	 */
	enum class BootInfoType : uint32_t {
		END = 0,
		BOOT_CMD_LINE = 1,
		BOOTLOADER_NAME = 2,
		// MODULES = 3,
		// BASIC_MEMORY_INFO = 4,
		// BIOS_BOOT_DEVICE = 5,
		MEMORY_MAP = 6,
		// VBE_INFO = 7,
		FRAMEBUFFER_INFO = 8,
		ELF_SYMBOLS = 9,
		// APM_TABLE = 10,
		// EFI_32_SYSTEM_TABLE = 11,
		// EFI_64_SYSTEM_TABLE = 12,
		// SMBIOS_TABLES = 13,
		ACPI_RSDP_1 = 14,
		ACPI_RSDP_2 = 15,
		// NETWORKING_INFO = 16,
		// EFI_MEMORY_MAP = 17,
		// EFI_BOOT_SERVICES_NOT_TERMINATED = 18,
		// EFI_32_IMAGE_HANDLE_PTR = 19,
		// EFI_64_IMAGE_HANDLE_PTR = 20,
		// EFI_IMAGE_LOAD_BASE_ADDRESS = 21
	};
	// TODO Enable more types as they are implemented

	/**
	 * @brief Generic string tag
	 *
	 */
	struct StringTag {
		uint32_t type;
		uint32_t size;
		char string[0];
	};

	/**
	 * @brief Memory map entry type
	 *
	 */
	enum class MemoryMapEntryType : uint32_t {
		AVAILABLE = 1,
		RESERVED = 2,
		ACPI_RECLAIMABLE = 3,
		ACPI_NVS = 4,
		BAD = 5
	};

	/**
	 * @brief Memory map entry
	 *
	 */
	struct MemoryMapEntry {
		uint64_t base;
		uint64_t length;
		MemoryMapEntryType type;
		unsigned : 32;
	};

	/**
	 * @brief Memory map
	 *
	 */
	struct MemoryMap {
		uint32_t type = 6;
		uint32_t size;
		uint32_t entry_size;
		uint32_t entry_ver;
		MemoryMapEntry entries[0];
	};

	/**
	 * @brief Framebuffer info
	 *
	 */
	struct FramebufferInfo {
		uint32_t type = 8;
		uint32_t size;
		uint64_t addr;
		uint32_t pitch;
		uint32_t width;
		uint32_t height;
		uint8_t bpp;
		uint8_t color_type;
		unsigned : 16;
		uint8_t red_field_pos;
		uint8_t red_mask_size;
		uint8_t green_field_pos;
		uint8_t green_mask_size;
		uint8_t blue_field_pos;
		uint8_t blue_mask_size;
	};

	/**
	 * @brief ELF symbols
	 *
	 */
	struct ELFSymbols {
		uint32_t type = 9;
		uint32_t size;
		uint32_t entry_num;
		uint32_t entry_size;
		uint32_t shndx;
		char sections[0];
	};

	/**
	 * @brief ACPI Root System Description Pointer
	 *
	 */
	struct ACPI_RSDP {
		uint32_t type; // 14 or 15
		uint32_t size;
		uint8_t rsdp[0];
	};

	/**
	 * @brief Initialize the multiboot2 boot info
	 *
	 * @param magic The magic number passed by a multiboot2 compliant bootloader
	 * @param addr The address of the multiboot2 info structure
	 */
	void init(uint32_t magic, void *addr);

	/**
	 * @brief Get a multiboot2 entry
	 *
	 * @param type The type of entry to get
	 * @return A pointer to the entry, or nullptr if not found
	 */
	[[nodiscard]] void const *get_entry(BootInfoType type);
}