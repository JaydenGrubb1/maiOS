/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-25
 * @brief Initialize and provide access to the framebuffer
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __arch_x86_64
#include <arch/x86_64/framebuffer.h>
#else
#error "Unsupported architecture"
#endif