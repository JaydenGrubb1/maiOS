/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-13
 * @brief // DOC
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

#include <string_view>

namespace ACPI {
	/**
	 * @brief System Description Table header
	 *
	 */
	struct SDTHeader {
		char signature[4];
		uint32_t length;
		uint8_t revision;
		uint8_t checksum;
		char oem_id[6];
		char oem_table_id[8];
		uint32_t oem_revision;
		uint32_t creator_id;
		uint32_t creator_revision;
		uint32_t data[0];
	} __attribute__((packed));

	/**
	 * @brief Initializes the Advanced Configuration and Power Interface
	 *
	 */
	void init(void);

	/**
	 * @brief Gets an ACPI entry by signature
	 *
	 * @param signature The signature of the entry to get
	 * @return A pointer to the entry
	 */
	[[nodiscard]] void const *get_entry(std::string_view signature);
}