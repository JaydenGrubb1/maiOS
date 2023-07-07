/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

/**
 * @brief Writes a single character to the UART
 *
 * @param c The character to write
 * @return The character that was written
 */
int kputchar(char c);

/**
 * @brief Writes a string to the UART
 *
 * @param str The string to write
 * @return The number of characters written
 */
int kputs(const char *str);

/**
 * @brief Writes a string to the UART up to a maximum length
 *
 * @param str The string to write
 * @param n The maximum number of characters to write
 * @return The number of characters written
 */
int kputns(const char *str, size_t n);

/**
 * @brief Writes a formatted string to the UART
 *
 * @param format The format string
 * @param ... The format arguments
 * @return The number of characters written
 */
int kprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));