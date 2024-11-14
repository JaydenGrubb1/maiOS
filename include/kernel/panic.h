/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-02-15
 * @brief Panic the kernel
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string_view>

namespace Kernel {
	/**
	 * @brief Panic the kernel
	 *
	 * @param msg The message to display
	 */
	[[noreturn]] void panic(std::string_view msg);
}