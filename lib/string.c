/**
 * @file lib/string.c
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief Various string and memory manipulation functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <lib/string.h>

/**********************************************************************
 * Standard C functions
 *********************************************************************/

int memcmp(const void *buf1, const void *buf2, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (((char *)buf1)[i] < ((char *)buf2)[i]) {
			return -1;
		} else if (((char *)buf1)[i] > ((char *)buf2)[i]) {
			return 1;
		}
	}

	return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)dest)[i] = ((char *)src)[i];
	}

	return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
	if (dest < src) {
		for (size_t i = 0; i < n; i++) {
			((char *)dest)[i] = ((char *)src)[i];
		}
	} else {
		for (size_t i = n; i > 0; i--) {
			((char *)dest)[i - 1] = ((char *)src)[i - 1];
		}
	}

	return dest;
}

void *memset(void *buf, int value, size_t n) {
	for (size_t i = 0; i < n; i++) {
		((char *)buf)[i] = value;
	}

	return buf;
}

size_t strlen(const char *str) {
	size_t len = 0;
	while (str[len]) {
		len++;
	}
	return len;
}

/**********************************************************************
 * Non-standard C functions
 *********************************************************************/

size_t strnlen(const char *str, size_t maxlen) {
	size_t len = 0;
	while (str[len] && len < maxlen) {
		len++;
	}
	return len;
}