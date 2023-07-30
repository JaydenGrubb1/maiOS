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

#ifdef __cplusplus
}
#endif