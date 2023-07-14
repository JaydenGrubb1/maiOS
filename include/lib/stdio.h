/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-13
 * @brief Standard input/output functions
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF (-1)

/**
 * @brief Print formatted output to stdout
 *
 * @param format The format string
 * @param ... The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html @endlink
 */
int printf(const char *format, ...) __attribute__((format(printf, 1, 2)));

/**
 * @brief Print a character to stdout
 *
 * @param c The character to print
 * @return The character printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/putchar.html @endlink
 */
int putchar(int c);

/**
 * @brief Print a string to stdout
 *
 * @param s The string to print
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/puts.html @endlink
 */
int puts(const char *s);

/**
 * @brief Print formatted output to a string buffer
 *
 * @param str The string buffer to print to
 * @param size The size of the string buffer
 * @param format The format string
 * @param ... The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html @endlink
 */
int snprintf(char *str, size_t size, const char *format, ...) __attribute__((format(printf, 3, 4)));

/**
 * @brief Print formatted output to a string buffer
 *
 * @param str The string buffer to print to
 * @param format The format string
 * @param ... The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html @endlink
 */
int sprintf(char *str, const char *format, ...) __attribute__((format(printf, 2, 3)));
;

/**
 * @brief Print formatted output to stdout
 *
 * @param format The format string
 * @param ap The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html @endlink
 */
int vprintf(const char *format, va_list ap) __attribute__((format(printf, 1, 0)));

/**
 * @brief Print formatted output to a string buffer
 *
 * @param str The string buffer to print to
 * @param size The size of the string buffer
 * @param format The format string
 * @param ap The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html @endlink
 */
int vsnprintf(char *str, size_t size, const char *format, va_list ap) __attribute__((format(printf, 3, 0)));

/**
 * @brief Print formatted output to a string buffer
 *
 * @param str The string buffer to print to
 * @param format The format string
 * @param ap The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html @endlink
 */
int vsprintf(char *str, const char *format, va_list ap) __attribute__((format(printf, 2, 0)));

#ifdef __cplusplus
}
#endif