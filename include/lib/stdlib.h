/**
 * @file include/lib/stdlib.h
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief General purpose standard C library
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
 * @brief Register a function to be called at program exit
 * 
 * @param function The function to call
 * @return 0 on success, -1 on failure
 */
int atexit(void (*function)(void));

/**
 * @brief Convert an integer to a string
 *
 * @param value The integer to convert
 * @param str The string to write to
 * @param base The base to use for the conversion
 * @return The string that was written to
 *
 * @note This function is not part of the C standard
 */
char *itoa(int value, char *str, int base);

#ifdef __cplusplus
}
#endif