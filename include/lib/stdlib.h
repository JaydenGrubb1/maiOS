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

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************
 * Standard C functions
 *********************************************************************/

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
int atoi(const char *str);

/**
 * @brief Convert a string to a long
 *
 * @param str The string to convert
 * @return The long that was converted
 * 
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html @endlink
 */
long atol(const char *str);

/**
 * @brief Convert a string to a long long
 *
 * @param str The string to convert
 * @return The long long that was converted
 * 
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html @endlink
 */
long long atoll(const char *str);

/**********************************************************************
 * Non-standard C functions
 *********************************************************************/

#define BINARY 2
#define OCTAL 8
#define DECIMAL 10
#define HEXADECIMAL 16

/**
 * @brief Convert a byte to a string
 *
 * @param value The byte to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *btoa(int8_t value, char *str, int base);

/**
 * @brief Convert an unsigned byte to a string
 *
 * @param value The byte to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *ubtoa(uint8_t value, char *str, int base);

/**
 * @brief Convert a short to a string
 *
 * @param value The short to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *stoa(int16_t value, char *str, int base);

/**
 * @brief Convert an unsigned short to a string
 *
 * @param value The short to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *ustoa(uint16_t value, char *str, int base);

/**
 * @brief Convert an integer to a string
 *
 * @param value The integer to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *itoa(int32_t value, char *str, int base);

/**
 * @brief Convert an unsigned integer to a string
 *
 * @param value The integer to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 *
 * @note This function is not part of the C standard
 */
char *uitoa(uint32_t value, char *str, int base);

/**
 * @brief Convert an long to a string
 *
 * @param value The long to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 */
char *ltoa(int64_t value, char *str, int base);

/**
 * @brief Convert an unsigned long to a string
 *
 * @param value The long to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to or NULL on failure
 */
char *ultoa(uint64_t value, char *str, int base);

#ifdef __cplusplus
}
#endif