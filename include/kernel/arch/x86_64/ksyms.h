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

#include <stdbool.h>
#include <stdint.h>

namespace KSyms {
	/**
	 * @brief Get the symbol name for a given address
	 *
	 * @param addr The address to get the symbol name for
	 * @param sym_addr The start address the symbol refers to
	 * @return The symbol name, or nullptr if not found
	 */
	[[nodiscard]] const char *get_symbol(void *addr, uintptr_t *sym_addr);
	// TODO return tuple<const char *, uintptr_t> get_symbol(void *addr);

	/**
	 * @brief Initialize the kernel symbol table
	 *
	 */
	void init(void);

	/**
	 * @brief Check if the kernel symbol table is available
	 *
	 * @return true if available, false otherwise
	 */
	[[nodiscard]] bool is_available(void);
}