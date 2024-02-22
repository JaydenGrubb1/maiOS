/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-18
 * @brief Implements the C++ ABI
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cxxabi.h>

#include <kernel/panic.h>

extern "C" void __cxa_pure_virtual(void) {
	Kernel::panic("Pure virtual function called");
}