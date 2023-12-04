/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Version information definitions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define __kernel_name "mai-OS"
#define __kernel_version_major 0
#define __kernel_version_minor 0
#define __kernel_version_patch 1

#ifdef __arch_x86_64
#define __kernel_arch "x86_64"
#else
#error "Unsupported architecture"
#endif

#define __kernel_build_date __DATE__
#define __kernel_build_time __TIME__
#define __kernel_compiler "gcc (" STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__) ")"
