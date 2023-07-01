/**
 * @file include/lib/ctype.h
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-01
 * @brief Character type functions
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
 * @brief Return the lowercase version of a c
 *
 * @param c The character to convert
 * @return The lowercase character
 */
int tolower(int c);

/**
 * @brief Return the uppercase version of a c
 *
 * @param c The character to convert
 * @return The uppercase character
 */
int toupper(int c);

#ifdef __cplusplus
}
#endif