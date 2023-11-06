/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief Various string and memory manipulation functions
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/string.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <lib/libc/string.h>

/**********************************************************************
 * Standard C functions
 *********************************************************************/

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memcmp.html
int memcmp(const void *buf1, const void *buf2, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (static_cast<const char *>(buf1)[i] < static_cast<const char *>(buf2)[i]) {
			return -1;
		} else if (static_cast<const char *>(buf1)[i] > static_cast<const char *>(buf2)[i]) {
			return 1;
		}
	}

	return 0;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memcpy.html
void *memcpy(void *__restrict__ dest, const void *__restrict__ src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		static_cast<char *>(dest)[i] = static_cast<const char *>(src)[i];
	}

	return dest;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memmove.html
void *memmove(void *dest, const void *src, size_t n) {
	if (dest < src) {
		for (size_t i = 0; i < n; i++) {
			static_cast<char *>(dest)[i] = static_cast<const char *>(src)[i];
		}
	} else {
		for (size_t i = n; i > 0; i--) {
			static_cast<char *>(dest)[i - 1] = static_cast<const char *>(src)[i - 1];
		}
	}

	return dest;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/memset.html
void *memset(void *buf, int value, size_t n) {
	for (size_t i = 0; i < n; i++) {
		static_cast<char *>(buf)[i] = value;
	}

	return buf;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/strlen.html
size_t strlen(const char *str) {
	size_t len = 0;
	while (str[len]) {
		len++;
	}
	return len;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/strlen.html
size_t strnlen(const char *str, size_t maxlen) {
	size_t len = 0;
	while (str[len] && len < maxlen) {
		len++;
	}
	return len;
}

/**********************************************************************
 * Non-standard C functions
 *********************************************************************/