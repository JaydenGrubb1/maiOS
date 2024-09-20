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

#include <bits/file.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EOF (-1)
#define BUFSIZ 4096

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

/**
 * @brief Write data from a buffer to a file stream
 *
 * @param ptr The buffer to write from
 * @param size The size of each element to write
 * @param num The number of elements to write
 * @param stream The file stream to write to
 * @return The number of elements written
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fwrite.html @endlink
 */
size_t fwrite(const void *ptr, size_t size, size_t num, FILE *stream);

/**
 * @brief Flush a file stream
 *
 * @param stream The file stream to flush
 * @return 0 on success, EOF on failure
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fflush.html @endlink
 */
int fflush(FILE *stream);

/**
 * @brief Check if a file stream is at the end of the file
 *
 * @param stream The file stream to check
 * @return Non-zero if the file stream is at the end of the file, zero otherwise
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/feof.html @endlink
 */
[[nodiscard]] int feof(FILE *stream);

/**
 * @brief Check if a file stream has an error
 *
 * @param stream The file stream to check
 * @return Non-zero if the file stream has an error, zero otherwise
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/ferror.html @endlink
 */
[[nodiscard]] int ferror(FILE *stream);

/**
 * @brief Clear the end-of-file and error indicators for a file stream
 *
 * @param stream The file stream to clear the indicators for
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/clearerr.html @endlink
 */
void clearerr(FILE *stream);

/**
 * @brief Get the file descriptor for a file stream
 *
 * @param stream The file stream to get the file descriptor for
 * @return The file descriptor for the file stream
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fileno.html @endlink
 */
[[nodiscard]] int fileno(FILE *stream);

/**
 * @brief Print a character to a file stream
 *
 * @param c The character to print
 * @param stream The file stream to print to
 * @return The character printed, or EOF on failure
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fputc.html @endlink
 */
int fputc(int c, FILE *stream);

/**
 * @brief Print a string to a file stream
 *
 * @param s The string to print
 * @param stream The file stream to print to
 * @return The number of characters printed, or EOF on failure
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/fputs.html @endlink
 */
int fputs(const char *s, FILE *stream);

/**
 * @brief Print a character to stdout
 *
 * @param c The character to print
 * @return The character printed, or EOF on failure
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

/**
 * @brief Print formatted output to a file stream
 *
 * @param stream The file stream to print to
 * @param format The format string
 * @param ... The arguments to be formatted
 * @return The number of characters printed
 */
int fprintf(FILE *stream, const char *format, ...) __attribute__((format(printf, 2, 3)));

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

/**
 * @brief Print formatted output to a file stream
 *
 * @param stream The file stream to print to
 * @param format The format string
 * @param ap The arguments to be formatted
 * @return The number of characters printed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html @endlink
 */
int vfprintf(FILE *stream, const char *format, va_list ap) __attribute__((format(printf, 2, 0)));

#ifdef __cplusplus
}
#endif