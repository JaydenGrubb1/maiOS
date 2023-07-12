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

namespace KSyms {
	/**
	 * @brief Get a symbol name for a given address
	 * 
	 * @param addr The address to get the symbol for
	 * @return The symbol name, or nullptr if not found
	 */
	const char *get_symbol(void *addr);

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
	bool is_available(void);
}