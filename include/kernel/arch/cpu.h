/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-05
 * @brief CPU related functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __arch_x86_64
#include <kernel/arch/x86_64/cpu.h>
#else
#error "Unsupported architecture"
#endif