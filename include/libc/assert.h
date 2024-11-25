/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Defines the assert macro
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/assert.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Print a failure message and halt the CPU if an assertion fails
 *
 * @param assertion The assertion that failed
 * @param file The file the assertion failed in
 * @param line The line the assertion failed on
 * @param function The function the assertion failed in
 */
void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function);

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/assert.html
#ifdef DNDEBUG
#define assert(expr) ((void)0)
#else
#ifdef __cplusplus
#define assert(expr)         \
	(static_cast<bool>(expr) \
		 ? void(0)           \
		 : __assert_fail(#expr, __FILE__, __LINE__, __extension__ __PRETTY_FUNCTION__))
#else
#define assert(expr) \
	((expr)          \
		 ? (void)(0) \
		 : __assert_fail(#expr, __FILE__, __LINE__, __func__))
#endif
#endif

#ifdef __cplusplus
}
#endif