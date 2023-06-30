/**
 * @file lib/stdlib.c
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

#include <lib/stdlib.h>

int atexit(void (*function)(void)) {
#ifdef __is_kernel
	// VERIFY Does this require ERRNO?
	return -1;
#else
	// TODO Implement this
	return -1;
#endif
}

char *itoa(int value, char *str, int base) {
	// TODO Implement this
	return str;
}