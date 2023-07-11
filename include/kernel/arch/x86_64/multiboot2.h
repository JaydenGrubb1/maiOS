/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-23
 * @brief Used to parse information from the Multiboot2 information block
 * @link https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html @endlink
 *
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define NUM_SEGMENTS 21

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Used to parse information from the Multiboot2 information block
 * @link https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html @endlink
 */
class Multiboot2 {
  public:
	/**
	 * @brief // DOC
	 *
	 */
	struct Module {
		uint32_t start;
		uint32_t end;
		char *string() {
			return &string_m;
		}

	  private:
		char string_m;
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct BasicMemoryInfo {
		uint32_t lower;
		uint32_t upper;
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct BiosBootDevice {
		uint32_t biosDevice;
		uint32_t partition;
		uint32_t subPartition;

	  private:
		uint32_t padding;
	};

	/**
	 * @brief // DOC
	 *
	 */
	enum MemoryMapEntryType : uint32_t {
		AVAILABLE = 1,
		RESERVED = 2,
		ACPI_RECLAIMABLE = 3,
		ACPI_NVS = 4,
		BAD = 5
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct MemoryMapEntry {
		uint64_t baseAddress;
		uint64_t length;
		MemoryMapEntryType type;

	  private:
		uint32_t padding;
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct MemoryMap {
		uint32_t entrySize;
		uint32_t entryVersion;
		MemoryMapEntry *getEntries() {
			return &entries_m;
		}
		size_t entryCount();

	  private:
		MemoryMapEntry entries_m;
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct VBEInfo {
		uint16_t mode;
		uint16_t interfaceSegment;
		uint16_t interfaceOff;
		uint16_t interfaceLength;
		uint8_t controlInfo[512];
		uint8_t modeInfo[256];
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct FramebufferInfo {
		uint64_t address;
		uint32_t pitch;
		uint32_t width;
		uint32_t height;
		uint8_t bpp;
		uint8_t type;
		uint8_t reserved;
		// TODO Framebuffer Color Info
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct ELFSymbols {
		uint16_t number;
		uint16_t entrySize;
		uint16_t shndx;
		uint16_t reserved;
		// TODO Elf Symbols Section Headers
	};

	/**
	 * @brief // DOC
	 *
	 */
	struct APMTable {
		uint16_t version;
		uint16_t cseg;
		uint32_t offset;
		uint16_t cseg16;
		uint16_t dseg;
		uint16_t flags;
		uint16_t csegLength;
		uint16_t cseg16Length;
		uint16_t dsegLength;
	};

	struct SMBIOSTables {
		uint8_t major;
		uint8_t minor;
		uint8_t reserved[6];
		// TODO SMBIOS Tables
	};

	// TODO ACPI 1.0 RSDP

	// TODO ACPI 2.0 RSDP

	// TODO Networking Info

	struct EFIMemoryMap {
		uint32_t size;
		uint32_t version;
		// TODO EFI Memory Map
	};

	/**
	 * @brief Type of info block to get
	 *
	 */
	enum BootInfoType {
		BOOT_CMD_LINE = 1,
		BOOTLOADER_NAME = 2,
		MODULES = 3,
		BASIC_MEMORY_INFO = 4,
		BIOS_BOOT_DEVICE = 5,
		MEMORY_MAP = 6,
		VBE_INFO = 7,
		FRAMEBUFFER_INFO = 8,
		ELF_SYMBOLS = 9,
		APM_TABLE = 10,
		EFI_32_SYSTEM_TABLE = 11,
		EFI_64_SYSTEM_TABLE = 12,
		SMBIOS_TABLES = 13,
		ACPI_RSDP_1 = 14,
		ACPI_RSDP_2 = 15,
		NETWORKING_INFO = 16,
		EFI_MEMORY_MAP = 17,
		EFI_BOOT_SERVICES_NOT_TERMINATED = 18,
		EFI_32_IMAGE_HANDLE_PTR = 19,
		EFI_64_IMAGE_HANDLE_PTR = 20,
		EFI_IMAGE_LOAD_BASE_ADDRESS = 21
	};

	/**
	 * @brief Initialize the Multiboot2 parser
	 * @param ptr Pointer to the Multiboot2 info block
	 * @return True if successful, false otherwise
	 */
	static bool init(uint8_t *ptr);

	/**
	 * @brief Gets the total size of the Multiboot2 info block
	 * @return The total size of the Multiboot2 info block
	 */
	static uint32_t totalSize();

	/**
	 * @brief Gets a pointer to a specific block of info
	 * @tparam T The type to cast the pointer to
	 * @param infoType The type of info block to get
	 * @return A pointer to a block of info
	 */
	template <typename T>
	static T *getPtr(BootInfoType infoType) {
		if (infoType == EFI_BOOT_SERVICES_NOT_TERMINATED) {
			return (T *)instance().segments_m[infoType];
		} else {
			if (instance().segments_m[infoType] == nullptr) {
				return nullptr;
			} else {
				return (T *)(instance().segments_m[infoType] + 8);
			}
		}
	}

  private:
	uint32_t totalSize_m;
	uint8_t *segments_m[NUM_SEGMENTS];

	/**
	 * @brief Gets a reference to the static instance of this class
	 * @return A reference to the static instance of this class
	 */
	static Multiboot2 &instance();
};