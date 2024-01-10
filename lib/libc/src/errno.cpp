/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-11
 * @brief System error numbers
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <errno.h>

int *__errno_location(void) {
	// TODO per thread errno
	static int value = 0;
	return &value;
}