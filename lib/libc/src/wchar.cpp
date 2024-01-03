/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-02
 * @brief Wide character handling
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/wchar.h.html @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <defines.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcrtomb.html
size_t wcrtomb(char *mb, wchar_t wc, UNUSED mbstate_t *state) {
	char temp[4];
	if (mb == nullptr) {
		mb = temp;
	}

	if (wc < 0x80) {
		mb[0] = wc;
		return 1;
	} else if (wc < 0x800) {
		mb[0] = 0xC0 | (wc >> 6);
		mb[1] = 0x80 | (wc & 0x3F);
		return 2;
	} else if (wc < 0x10000) {
		mb[0] = 0xE0 | (wc >> 12);
		mb[1] = 0x80 | ((wc >> 6) & 0x3F);
		mb[2] = 0x80 | (wc & 0x3F);
		return 3;
	} else if (wc < 0x200000) {
		mb[0] = 0xF0 | (wc >> 18);
		mb[1] = 0x80 | ((wc >> 12) & 0x3F);
		mb[2] = 0x80 | ((wc >> 6) & 0x3F);
		mb[3] = 0x80 | (wc & 0x3F);
		return 4;
	} else {
		return -1;
	}
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbrtowc.html
size_t mbrtowc(wchar_t *wc, const char *mb, size_t n, UNUSED mbstate_t *state) {
	if (mb == nullptr) {
		return 0;
	}
	if (n == 0) {
		return -2;
	}

	size_t count = 0;
	char head = *mb;
	while (head & 0x80) {
		count++;
		head <<= 1;
	}

	if (count == 0) {
		*wc = mb[0];
		return 1;
	}

	if (count > n) {
		return -2;
	}

	if (wc == nullptr) {
		return count;
	}

	for (size_t byte = 0; byte < count; byte++) {
		char mask = 0x3F;
		if (byte == 0) {
			mask = 0xFF >> (count + 1);
		} else if ((mb[byte] & 0xC0) != 0x80) {
			return -1;
		}
		*wc |= (mb[byte] & mask) << (count - byte - 1) * 6;
	}

	return count;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcsrtombs.html
size_t wcsnrtombs(char *dest, const wchar_t **src, size_t len, size_t max, mbstate_t *state) {
	char *ptr = dest;
	for (size_t i = 0; i < len; i++) {
		char temp[MB_CUR_MAX];
		size_t ret = wcrtomb(temp, (*src)[i], state);

		if (ret == -1UL) {
			return -1;
		} else if (ret > max) {
			return -1;
		}

		if (dest) {
			memcpy(ptr, temp, ret);
		}
		if ((*src)[i] == L'\0') {
			return ptr - dest;
		}

		ptr += ret;
		max -= ret;
	}
	return ptr - dest;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcsrtombs.html
size_t wcsrtombs(char *dest, const wchar_t **src, size_t max, mbstate_t *state) {
	char *ptr = dest;
	while (true) {
		char temp[MB_CUR_MAX];
		size_t ret = wcrtomb(temp, **src, state);

		if (ret == -1UL) {
			return -1;
		} else if (ret > max) {
			return ptr - dest;
		}

		if (dest) {
			memcpy(ptr, temp, ret);
		}
		if (temp[0] == '\0') {
			return ptr - dest;
		}

		ptr += ret;
		max -= ret;
		(*src)++;
	}
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbsrtowcs.html
size_t mbsnrtowcs(wchar_t *dest, const char **src, size_t len, size_t max, mbstate_t *state) {
	for (size_t i = 0; i < len; i++) {
		wchar_t temp;
		size_t size = MB_CUR_MAX < max ? MB_CUR_MAX : max;
		size_t ret = mbrtowc(&temp, *src, size, state);

		if (ret == -1UL) {
			return -1;
		} else if (ret == -2UL) {
			return -1;
		}

		if (dest) {
			dest[i] = temp;
		}
		if (temp == L'\0') {
			return i;
		}

		*src += ret;
		max -= ret;
	}
	return len;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbsrtowcs.html
size_t mbsrtowcs(wchar_t *dest, const char **src, size_t max, mbstate_t *state) {
	size_t i = 0;
	while (true) {
		wchar_t temp;
		size_t size = MB_CUR_MAX < max ? MB_CUR_MAX : max;
		size_t ret = mbrtowc(&temp, *src, size, state);

		if (ret == -1UL || ret == -2UL) {
			return -1;
		}

		if (dest) {
			dest[i] = temp;
		}
		if (temp == L'\0') {
			return i;
		}

		*src += ret;
		max -= ret;
		i++;
	}
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcslen.html
size_t wcslen(const wchar_t *str) {
	size_t len = 0;
	while (str[len]) {
		len++;
	}
	return len;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/wcslen.html
size_t wcsnlen(const wchar_t *str, size_t maxlen) {
	size_t len = 0;
	while (str[len] && len < maxlen) {
		len++;
	}
	return len;
}