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
#define ATTRIBUTE(...) __attribute__((__VA_ARGS__))
#define ERROR(...) __attribute__((error(__VA_ARGS__)))
#define FORMAT(...) __attribute__((format(__VA_ARGS__)))
#define NONNULL(...) __attribute__((nonnull(__VA_ARGS__)))
#define PACKED __attribute__((packed))
#define PURE __attribute__((pure))
#define SECTION(x) __attribute__((section(x)))
#define USED __attribute__((used))
#define WARNING(...) __attribute__((warning(__VA_ARGS__)))
// #define ALLOC_ALIGN(...) __attribute__((alloc_align(__VA_ARGS__)))
// #define ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
// #define ASSUME_ALIGNED(...) __attribute__((assume_aligned(__VA_ARGS__)))
// #define MALLOC(...) __attribute__((malloc(__VA_ARGS__)))

#define KiB (1024)
#define MiB (1024 * KiB)
#define GiB (1024 * MiB)
#define TiB (1024 * GiB)
#define PiB (1024 * TiB)
#define EiB (1024 * PiB)

#define __FILE_BASENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)