/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-01
 * @brief Character type functions
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/ctype.h.html @endlink
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
 * @brief Check if a character is a digit
 *
 * @param c The character to check
 * @return True if the character is a digit, false otherwise
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/isdigit.html @endlink
 */
inline int isdigit(int c) {
	return c >= '0' && c <= '9';
}

/**
 * @brief Return the lowercase version of a c
 *
 * @param c The character to convert
 * @return The lowercase character
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/tolower.html @endlink
 */
int tolower(int c);

/**
 * @brief Return the uppercase version of a c
 *
 * @param c The character to convert
 * @return The uppercase character
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/toupper.html @endlink
 */
int toupper(int c);

#ifdef __cplusplus
}
#endif