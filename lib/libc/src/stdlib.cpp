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

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atexit.html
int atexit(__attribute__((unused)) void (*function)(void)) {
#ifdef __is_kernel
	// VERIFY Does this require ERRNO?
	return -1;
#else
	// TODO Implement this
	// TODO Remove UNUSED
	return -1;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/abort.html
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

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atoi.html
int atoi(const char *str) {
	return _ato<int>(str);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html
long atol(const char *str) {
	return _ato<long>(str);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/atol.html
long long atoll(const char *str) {
	return _ato<long long>(str);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html
void *malloc(size_t size) {
#ifdef __is_kernel
	return Memory::allocate(size, alignof(max_align_t));
#else
	// TODO Implement this
	return nullptr;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/calloc.html
void *calloc(size_t num_elem, size_t size_elem) {
#ifdef __is_kernel
	return Memory::allocate(num_elem * size_elem, alignof(max_align_t), true);
#else
	// TODO Implement this
	return nullptr;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/realloc.html
void *realloc(void *ptr, size_t size) {
#ifdef __is_kernel
	// TODO Maybe add a Memory::reallocate function
	auto temp = Memory::allocate(size, alignof(max_align_t));
	if (temp) {
		memmove(temp, ptr, size);
		Memory::deallocate(ptr);
	}
	return temp;
#else
	// TODO Implement this
	return nullptr;
#endif
}

// DOC find documentation link (closest I could find was posix_memalign)
void *aligned_alloc(size_t alignment, size_t size) {
#ifdef __is_kernel
	return Memory::allocate(size, alignment);
#else
	// TODO Implement this
	return nullptr;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html
void free(void *ptr) {
#ifdef __is_kernel
	Memory::deallocate(ptr);
#else
	// TODO Implement this
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wctomb.html
int wctomb(char *mb, wchar_t wc) {
	static mbstate_t state;
	if (mb == nullptr) {
		memset(&state, 0, sizeof(mbstate_t));
		return 0;
	}

	return wcrtomb(mb, wc, &state);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbtowc.html
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

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcstombs.html
size_t wcstombs(char *dest, const wchar_t *src, size_t n) {
	static mbstate_t state;
	return wcsrtombs(dest, &src, n, &state);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbstowcs.html
size_t mbstowcs(wchar_t *dest, const char *src, size_t n) {
	static mbstate_t state;
	return mbsrtowcs(dest, &src, n, &state);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/rand.html
int rand(void) {
	return rand_r(&_seed);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/rand.html
int rand_r(unsigned int *seed) {
	*seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
	return *seed;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/rand.html
void srand(unsigned int seed) {
	_seed = seed;
}