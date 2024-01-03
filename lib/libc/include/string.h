/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief Various string and memory manipulation functions
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/string.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define __need_size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Copies bytes from src to dest until c is found or n bytes have been copied
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param c The character to search for
 * @param n The maximum number of bytes to copy
 * @return A pointer to the byte after c in dest, or NULL if c was not found
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memccpy.html @endlink
 */
void *memccpy(void *dest, const void *src, int c, size_t n);

/**
 * @brief Finds the first occurrence of c in buf
 *
 * @param buf The buffer to search
 * @param c The character to search for
 * @param n The number of bytes to search
 * @return A pointer to the first occurrence of c in buf, or NULL if c was not found
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memchr.html @endlink
 */
[[nodiscard]] void *memchr(const void *buf, int c, size_t n);

/**
 * @brief Compares the first n bytes of buf1 and buf2
 *
 * @param buf1 The first buffer
 * @param buf2 The second buffer
 * @param n The number of bytes to compare
 * @retval -1 buf1 is less than buf2
 * @retval 0 buf1 is equal to buf2
 * @retval 1 buf1 is greater than buf2
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memcmp.html @endlink
 */
[[nodiscard]] int memcmp(const void *buf1, const void *buf2, size_t n);

/**
 * @brief Copies n bytes from src to dest
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The number of bytes to copy
 * @return The destination buffer
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memcpy.html @endlink
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
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memmove.html @endlink
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
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/memset.html @endlink
 */
void *memset(void *buf, int value, size_t n);

/**
 * @brief Finds the length of a string
 *
 * @param str The string to find the length of
 * @return The length of the string
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/strlen.html @endlink
 */
[[nodiscard]] size_t strlen(const char *str);

/**
 * @brief Finds the length of a string, up to maxlen
 *
 * @param str The string to find the length of
 * @param maxlen The maximum length of the string
 * @return The length of the string
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/strlen.html @endlink
 */
[[nodiscard]] size_t strnlen(const char *str, size_t maxlen);

/**
 * @brief Split a string into tokens
 *
 * @param str The string to split
 * @param delim The delimiter to split on
 * @return A pointer to the next token, or NULL if there are no more tokens
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/strtok.html @endlink
 */
[[nodiscard]] char *strtok(char *str, const char *delim);

/**
 * @brief Split a string into tokens (reentrant)
 *
 * @param str The string to split
 * @param delim The delimiter to split on
 * @param saveptr A pointer to a char * that is used internally to keep track of the current position
 * @return A pointer to the next token, or NULL if there are no more tokens
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/strtok.html @endlink
 */
[[nodiscard]] char *strtok_r(char *str, const char *delim, char **saveptr);

#ifdef __cplusplus
}
#endif