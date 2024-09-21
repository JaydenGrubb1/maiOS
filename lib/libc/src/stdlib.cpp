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

#include <stddef.h>
#include <stdint.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#ifdef __is_kernel
#include <kernel/arch/memory.h>
#include <kernel/panic.h>
#endif

static unsigned int _seed = 1;

int atexit(void (*function)(void)) {
#ifdef __is_kernel
	// VERIFY Does this require ERRNO?
	(void)function;
	return -1;
#else
	// TODO Implement this
	return -1;
#endif
}

void abort(void) {
#ifdef __is_kernel
	Kernel::panic("abort() called");
#else
// TODO Implement this
#endif
	__builtin_unreachable();
}

/**
 * @brief Convert a string to an integer
 *
 * @tparam T The type of integer to convert to
 * @param str The string to convert
 * @return T The converted integer
 */
template <typename T>
__attribute__((always_inline)) inline T _ato(const char *str) {
	T result = 0;
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

int atoi(const char *str) {
	return _ato<int>(str);
}

long atol(const char *str) {
	return _ato<long>(str);
}

long long atoll(const char *str) {
	return _ato<long long>(str);
}

int wctomb(char *mb, wchar_t wc) {
	static mbstate_t state;
	if (mb == nullptr) {
		memset(&state, 0, sizeof(mbstate_t));
		return 0;
	}

	return wcrtomb(mb, wc, &state);
}

int mbtowc(wchar_t *wc, const char *mb, size_t n) {
	static mbstate_t state;
	if (mb == nullptr) {
		memset(&state, 0, sizeof(mbstate_t));
		return 0;
	}

	int ret = mbrtowc(wc, mb, n, &state);

	if (ret < 0) {
		return -1;
	} else {
		return ret;
	}
}

size_t wcstombs(char *dest, const wchar_t *src, size_t n) {
	static mbstate_t state;
	return wcsrtombs(dest, &src, n, &state);
}

size_t mbstowcs(wchar_t *dest, const char *src, size_t n) {
	static mbstate_t state;
	return mbsrtowcs(dest, &src, n, &state);
}

int rand(void) {
	return rand_r(&_seed);
}

int rand_r(unsigned int *seed) {
	*seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
	return *seed;
}

void srand(unsigned int seed) {
	_seed = seed;
}