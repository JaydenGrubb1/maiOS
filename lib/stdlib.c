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
#include <stddef.h>
#include <stdint.h>

/**********************************************************************
 * Standard C functions
 *********************************************************************/

int atexit(void (*function)(void)) {
#ifdef __is_kernel
	// VERIFY Does this require ERRNO?
	return -1;
#else
	// TODO Implement this
	return -1;
#endif
}

/**********************************************************************
 * Non-standard C functions
 *********************************************************************/

static const char *const digits = "zyxwvutsrqponmlkjihgfedcba987654321\
0123456789abcdefghijklmnopqrstuvwxyz";

char *itoa(int32_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	int32_t tmpv;

	do {
		tmpv = value;
		value /= base;
		*ptr++ = digits[35 + (tmpv - value * base)];
	} while (value);

	if (tmpv < 0)
		*ptr++ = '-';
	*ptr-- = '\0';

	char *tmpp = str;
	char tmpc;
	while (tmpp < ptr) {
		tmpc = *ptr;
		*ptr-- = *tmpp;
		*tmpp++ = tmpc;
	}

	return str;
}

char *uitoa(uint32_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	uint32_t tmpv;

	do {
		tmpv = value;
		value /= base;
		*ptr++ = digits[35 + (tmpv - value * base)];
	} while (value);
	*ptr-- = '\0';

	char *tmpp = str;
	char tmpc;
	while (tmpp < ptr) {
		tmpc = *ptr;
		*ptr-- = *tmpp;
		*tmpp++ = tmpc;
	}
	return str;
}