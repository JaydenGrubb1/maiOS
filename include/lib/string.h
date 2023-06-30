/**
 * @file include/lib/string.h
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief Various string and memory manipulation functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copies n bytes from src to dest
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The number of bytes to copy
 * @return The destination buffer
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Finds the length of a string
 *
 * @param str The string to find the length of
 * @return The length of the string
 */
size_t strlen(const char *str);

#ifdef __cplusplus
}
#endif