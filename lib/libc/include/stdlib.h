/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief General purpose standard C library
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdlib.h.html @endlink
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

#define MB_CUR_MAX sizeof(wchar_t)

/**
 * @brief Register a function to be called at program exit
 *
 * @param function The function to call
 * @return 0 on success, -1 on failure
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atexit.html @endlink
 */
int atexit(void (*function)(void));

/**
 * @brief Convert a string to an integer
 *
 * @param str The string to convert
 * @return The integer that was converted
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atoi.html @endlink
 */
[[nodiscard]] int atoi(const char *str);

/**
 * @brief Convert a string to a long
 *
 * @param str The string to convert
 * @return The long that was converted
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html @endlink
 */
[[nodiscard]] long atol(const char *str);

/**
 * @brief Convert a string to a long long
 *
 * @param str The string to convert
 * @return The long long that was converted
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html @endlink
 */
[[nodiscard]] long long atoll(const char *str);

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html @endlink
 */
[[nodiscard]] void *malloc(size_t size);

/**
 * @brief Allocate memory and set it to zero
 *
 * @param num_elem The number of elements to allocate
 * @param size_elem The size of each element
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/calloc.html @endlink
 */
[[nodiscard]] void *calloc(size_t num_elem, size_t size_elem);

/**
 * @brief Reallocate memory
 *
 * @param ptr The pointer to the memory to reallocate
 * @param size The new size of the memory
 * @return A pointer to the reallocated memory, or nullptr if the reallocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/realloc.html @endlink
 */
[[nodiscard]] void *realloc(void *ptr, size_t size);

/**
 * @brief Allocate aligned memory
 *
 * @param alignment The alignment of the memory
 * @param size The size of the memory
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 *
 * @link // DOC find documentation link (closest I could find was posix_memalign) @endlink
 */
[[nodiscard]] void *aligned_alloc(size_t alignment, size_t size);

/**
 * @brief Deallocate memory
 *
 * @param ptr A pointer to the memory to deallocate
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html @endlink
 */
void free(void *ptr);

/**
 * @brief Convert a wide character to a multibyte character
 *
 * @param mb The destination multibyte character
 * @param wc The source wide character
 * @return The number of bytes written, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wctomb.html @endlink
 */
int wctomb(char *mb, wchar_t wc);

/**
 * @brief Convert a multibyte character to a wide character
 *
 * @param wc The destination wide character
 * @param mb The source multibyte character
 * @param n The number of bytes in the multibyte character
 * @return The number of bytes read from the multibyte character, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbtowc.html @endlink
 */
int mbtowc(wchar_t *wc, const char *mb, size_t n);

/**
 * @brief Convert a wide character string to a multibyte character string
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The maximum number of bytes to write
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcstombs.html @endlink
 */
size_t wcstombs(char *dest, const wchar_t *src, size_t n);

/**
 * @brief Convert a multibyte character string to a wide character string
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param n The maximum number of bytes to write
 * @return The number of destination elements modified, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbstowcs.html @endlink
 */
size_t mbstowcs(wchar_t *dest, const char *src, size_t n);

#ifdef __cplusplus
}
#endif