/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief General purpose standard C library
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdlib.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <lib/ctype.h>
#include <lib/stdlib.h>
#include <stddef.h>
#include <stdint.h>

/**********************************************************************
 * Standard C functions
 *********************************************************************/

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atexit.html
int atexit(void (*function)(void)) {
#ifdef __is_kernel
	// VERIFY Does this require ERRNO?
	return -1;
#else
	// TODO Implement this
	return -1;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atoi.html
int atoi(const char *str) {
	int result = 0;
	int sign = 1;

	while (*str == ' ') {
		str++;
	}

	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	while (isdigit(*str)) {
		result *= 10;
		result += *str - '0';
		str++;
	}

	return result * sign;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html
long atol(const char *str) {
	long result = 0;
	int sign = 1;

	while (*str == ' ') {
		str++;
	}

	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	while (isdigit(*str)) {
		result *= 10;
		result += *str - '0';
		str++;
	}

	return result * sign;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html
long long atoll(const char *str) {
	long long result = 0;
	int sign = 1;

	while (*str == ' ') {
		str++;
	}

	if (*str == '-') {
		sign = -1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	while (isdigit(*str)) {
		result *= 10;
		result += *str - '0';
		str++;
	}

	return result * sign;
}

/**********************************************************************
 * Non-standard C functions
 *********************************************************************/

static const char *const digits = "zyxwvutsrqponmlkjihgfedcba987654321\
0123456789abcdefghijklmnopqrstuvwxyz";

char *btoa(int8_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	int8_t tmpv;

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

char *ubtoa(uint8_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	uint8_t tmpv;

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

char *stoa(int16_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	int16_t tmpv;

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

char *ustoa(uint16_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	uint16_t tmpv;

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

char *ltoa(int64_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	int64_t tmpv;

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

char *ultoa(uint64_t value, char *str, int base) {
	if (base < 2 || base > 36) {
		*str = '\0';
		return NULL;
	}

	char *ptr = str;
	uint64_t tmpv;

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