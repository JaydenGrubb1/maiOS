/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-30
 * @brief Various defines and constants used throughout the kernel
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define ALIGNED(x) __attribute__((aligned(x)))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define ERROR(...) __attribute__((error(__VA_ARGS__)))
#define FORMAT(...) __attribute__((format(__VA_ARGS__)))
#define NORETURN __attribute__((noreturn))
#define PACKED __attribute__((packed))
#define SECTION(x) __attribute__((section(x)))
#define UNUSED __attribute__((unused))

#define KiB (1024)
#define MiB (1024 * KiB)
#define GiB (1024 * MiB)
#define TiB (1024 * GiB)
#define PiB (1024 * TiB)
#define EiB (1024 * PiB)