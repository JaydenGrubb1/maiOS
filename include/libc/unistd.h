/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-20
 * @brief UNIX standard functions and constants
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

typedef long int ssize_t; // TODO move definition somewhere else

/**
 * @brief Attempt to write count bytes from buf to the file descriptor fd
 *
 * @param fd The file descriptor to write to
 * @param buf The buffer to write from
 * @param count The number of bytes to write
 * @return The number of bytes written, or -1 on error
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/pwrite.html @endlink
 */
ssize_t write(int fd, const void *buf, size_t count);