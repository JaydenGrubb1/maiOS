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
 * @brief Compares the first n bytes of buf1 and buf2
 *
 * @param buf1 The first buffer
 * @param buf2 The second buffer
 * @param n The number of bytes to compare
 * @retval -1 buf1 is less than buf2
 * @retval 0 buf1 is equal to buf2
 * @retval 1 buf1 is greater than buf2
 */
int memcmp(const void *buf1, const void *buf2, size_t n);

/**
 * @brief Copies n bytes from src to dest
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The number of bytes to copy
 * @return The destination buffer
 *
 * @note This function does not handle overlapping buffers
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * @brief Copies n bytes from src to dest
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The number of bytes to copy
 * @return The destination buffer
 *
 * @note This function does handle overlapping buffers
 */
void *memmove(void *dest, const void *src, size_t n);

/**
 * @brief Sets the first n bytes of buf to value
 *
 * @param buf The buffer to set
 * @param value The value to set the buffer to
 * @param n The number of bytes to set
 * @return The buffer that was set
 */
void *memset(void *buf, int value, size_t n);

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