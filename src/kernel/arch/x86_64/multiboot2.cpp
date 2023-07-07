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

#include <kernel/arch/x86_64/multiboot2.hpp>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Gets a unsigned 32-bit integer from a pointer
 * @param ptr The pointer to get the value from
 * @return An unsigned 32-bit integer
 */
inline uint32_t toVal(uint8_t *ptr) {
	return *((uint32_t *)ptr);
}

/**
 * @brief Gets a reference to the static instance of this class
 * @return A reference to the static instance of this class
 */
Multiboot2 &Multiboot2::instance() {
	static Multiboot2 inst;
	return inst;
}

/**
 * @brief Initialize the Multiboot2 parser
 * @param ptr Pointer to the Multiboot2 info block
 * @return True if successful, false otherwise
 */
bool Multiboot2::init(uint8_t *ptr) {
	for (size_t i = 0; i < NUM_SEGMENTS; i++) {
		instance().segments_m[i] = nullptr;
	}

	instance().totalSize_m = toVal(ptr);
	ptr += 8;

	uint32_t segType = toVal(ptr);
	uint32_t sizeCount = 0;
	while (segType != 0) {
		if (sizeCount > instance().totalSize_m || segType > NUM_SEGMENTS) {
			return false;
		}

		instance().segments_m[segType] = ptr;
		uint32_t segSize = toVal(ptr + 4);
		uint32_t offset = segSize + (8 - (segSize % 8));
		ptr += offset;
		sizeCount += offset;
		segType = toVal(ptr);
	}

	return true;
}

/**
 * @brief Gets the total size of the Multiboot2 info block
 * @return The total size of the Multiboot2 info block
 */
uint32_t Multiboot2::totalSize() {
	return instance().totalSize_m;
}

/**
 * @brief Gets the number of entries in the memory map
 * @return The number of entries in the memory map
 */
size_t Multiboot2::MemoryMap::entryCount() {
	uint8_t *ptr = instance().segments_m[MEMORY_MAP];
	uint32_t segSize = toVal(ptr + 4);
	uint32_t entrySize = toVal(ptr + 8);
	return (segSize - 16) / entrySize;
}