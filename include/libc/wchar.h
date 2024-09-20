/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-02
 * @brief Wide character handling
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/wchar.h.html @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

#include <bits/mbstate.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert a wide character to a multibyte character (restartable)
 *
 * @param mb The destination multibyte character
 * @param wc The source wide character
 * @param state The current state of the conversion
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcrtomb.html @endlink
 */
size_t wcrtomb(char *mb, wchar_t wc, mbstate_t *state);

/**
 * @brief Convert a multibyte character to a wide character (restartable)
 *
 * @param wc The destination wide character
 * @param mb The source multibyte character
 * @param n The number of bytes in the multibyte character
 * @param state The current state of the conversion
 * @return The number of bytes read from the multibyte character, -2 if the multibyte character incomplete, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbrtowc.html @endlink
 */
size_t mbrtowc(wchar_t *wc, const char *mb, size_t n, mbstate_t *state);

/**
 * @brief Convert a wide character string to a multibyte character string (restartable)
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param len The length of the source buffer
 * @param max The maximum number of bytes to write
 * @param state The current state of the conversion
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcsrtombs.html @endlink
 */
size_t wcsnrtombs(char *dest, const wchar_t **src, size_t len, size_t max, mbstate_t *state);

/**
 * @brief Convert a wide character string to a multibyte character string (restartable)
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param max The maximum number of bytes to write
 * @param state The current state of the conversion
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcsrtombs.html @endlink
 */
size_t wcsrtombs(char *dest, const wchar_t **src, size_t max, mbstate_t *state);

/**
 * @brief Convert a multibyte character string to a wide character string (restartable)
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param len The length of the source buffer
 * @param max The maximum number of bytes to write
 * @param state The current state of the conversion
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbsrtowcs.html @endlink
 */
size_t mbsnrtowcs(wchar_t *dest, const char **src, size_t len, size_t max, mbstate_t *state);

/**
 * @brief Convert a multibyte character string to a wide character string (restartable)
 *
 * @param dest The destination buffer
 * @param src The source buffer
 * @param max The maximum number of bytes to write
 * @param state The current state of the conversion
 * @return The number of bytes written to the buffer, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbsrtowcs.html @endlink
 */
size_t mbsrtowcs(wchar_t *dest, const char **src, size_t max, mbstate_t *state);

/**
 * @brief Find the length of a wide string
 *
 * @param str The string to find the length of
 * @return The length of the string
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcslen.html @endlink
 */
[[nodiscard]] size_t wcslen(const wchar_t *str);

/**
 * @brief Find the length of a wide string, up to maxlen
 *
 * @param str The string to find the length of
 * @param maxlen The maximum length of the string
 * @return The length of the string
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcslen.html @endlink
 */
[[nodiscard]] size_t wcsnlen(const wchar_t *str, size_t maxlen);

#ifdef __cplusplus
}
#endif