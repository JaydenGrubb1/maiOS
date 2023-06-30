/**
 * @file include/kernel/kprintf.hpp
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

/**
 * @brief Writes a formatted string to the UART
 *
 * @param format The format string
 * @param ... The format arguments
 * @return The number of characters written
 */
int kprintf(const char *format, ...);