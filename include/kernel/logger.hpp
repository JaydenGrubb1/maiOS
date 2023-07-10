/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Various wrappers for kprintf for debugging purposes
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <kernel/kprintf.hpp>

#ifdef __no_kernel_logging
#define LOG_RAW(fmt, ...)
#define LOG(fmt, ...)
#define LOG_INFO(fmt, ...)
#define LOG_PASS(fmt, ...)
#define LOG_WARN(fmt, ...)
#define LOG_FAIL(fmt, ...)
#else
#define LOG_RAW(fmt, ...) kprintf(fmt, ##__VA_ARGS__)
#define LOG(fmt, ...) kprintf("         " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) kprintf("[\u001b[36m INFO \u001b[0m] " fmt "\n", ##__VA_ARGS__)
#define LOG_PASS(fmt, ...) kprintf("[\u001b[32m  OK  \u001b[0m] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) kprintf("[\u001b[33m WARN \u001b[0m] " fmt "\n", ##__VA_ARGS__)
#define LOG_FAIL(fmt, ...) kprintf("[\u001b[31m FAIL \u001b[0m] " fmt "\n", ##__VA_ARGS__)
#endif

// TODO Add log to file
// TODO Maybe make actual logging functions