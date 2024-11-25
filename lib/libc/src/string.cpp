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

#include <stdlib.h>
#include <string.h>

void *memccpy(void *dest, const void *src, int c, size_t n) {
	for (size_t i = 0; i < n; i++) {
		static_cast<char *>(dest)[i] = static_cast<const char *>(src)[i];
		if (static_cast<const char *>(src)[i] == c) {
			return static_cast<char *>(dest) + i + 1;
		}
	}

	return nullptr;
}

void *memchr(const void *buf, int c, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (static_cast<const char *>(buf)[i] == c) {
			return const_cast<char *>(static_cast<const char *>(buf)) + i;
		}
	}

	return nullptr;
}

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

void *memcpy(void *dest, const void *src, size_t n) {
	for (size_t i = 0; i < n; i++) {
		static_cast<char *>(dest)[i] = static_cast<const char *>(src)[i];
	}

	return dest;
}

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

void *memset(void *buf, int value, size_t n) {
	for (size_t i = 0; i < n; i++) {
		static_cast<char *>(buf)[i] = value;
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

size_t strnlen(const char *str, size_t maxlen) {
	size_t len = 0;
	while (str[len] && len < maxlen) {
		len++;
	}
	return len;
}

int strcmp(const char *str1, const char *str2) {
	while (*str1 && *str2 && *str1 == *str2) {
		str1++;
		str2++;
	}

	return *str1 - *str2;
}

int strncmp(const char *str1, const char *str2, size_t n) {
	while (n > 0 && *str1 && *str2 && *str1 == *str2) {
		str1++;
		str2++;
		n--;
	}

	if (n == 0) {
		return 0;
	}
	return *str1 - *str2;
}

char *strtok(char *str, const char *delim) {
	static char *saveptr = nullptr;
	return strtok_r(str, delim, &saveptr);
}

char *strtok_r(char *str, const char *delim, char **saveptr) {
	if (str == nullptr) {
		str = *saveptr;
	}

	if (str == nullptr) {
		return nullptr;
	}

	size_t end = 0;
	while (str[end]) {
		bool found = false;
		for (size_t i = 0; delim[i]; i++) {
			if (str[end] == delim[i]) {
				found = true;
				break;
			}
		}

		if (found) {
			str[end] = '\0';
			*saveptr = str + end + 1;
			return str;
		}

		end++;
	}

	*saveptr = nullptr;
	return str;
}

char *strchr(const char *str, int c) {
	while (*str) {
		if (*str == c) {
			return const_cast<char *>(str);
		}
		str++;
	}

	return nullptr;
}

char *strrchr(const char *str, int c) {
	const char *last = nullptr;
	while (*str) {
		if (*str == c) {
			last = str;
		}
		str++;
	}

	return const_cast<char *>(last);
}

char *strstr(const char *str, const char *sub) {
	size_t sublen = strlen(sub);
	while (*str) {
		if (strncmp(str, sub, sublen) == 0) {
			return const_cast<char *>(str);
		}
		str++;
	}

	return nullptr;
}

char *strcpy(char *dest, const char *src) {
	size_t i = 0;
	while (src[i]) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
	size_t i = 0;
	while (i < n && src[i]) {
		dest[i] = src[i];
		i++;
	}
	while (i < n) {
		dest[i] = '\0';
		i++;
	}
	return dest;
}

char *stpcpy(char *dest, const char *src) {
	size_t i = 0;
	while (src[i]) {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return dest + i;
}

char *stpncpy(char *dest, const char *src, size_t n) {
	size_t i = 0;
	while (i < n && src[i]) {
		dest[i] = src[i];
		i++;
	}
	while (i < n) {
		dest[i] = '\0';
		i++;
	}
	return dest + i;
}

char *strdup(const char *str) {
	size_t len = strlen(str);
	char *dup = static_cast<char *>(malloc(len + 1));
	if (dup == nullptr) {
		return nullptr;
	}
	strcpy(dup, str);
	return dup;
}

char *strndup(const char *str, size_t n) {
	size_t len = strnlen(str, n);
	char *dup = static_cast<char *>(malloc(len + 1));
	if (dup == nullptr) {
		return nullptr;
	}
	strncpy(dup, str, len);
	dup[len] = '\0';
	return dup;
}

char *strpbrk(const char *str, const char *chars) {
	while (*str) {
		for (size_t i = 0; chars[i]; i++) {
			if (*str == chars[i]) {
				return const_cast<char *>(str);
			}
		}
		str++;
	}
	return nullptr;
}

char *strcat(char *s1, const char *s2) {
	size_t len = strlen(s1);
	strcpy(s1 + len, s2);
	return s1;
}

char *strncat(char *s1, const char *s2, size_t n) {
	size_t len = strlen(s1);
	strncpy(s1 + len, s2, n);
	s1[len + n] = '\0';
	return s1;
}

size_t strspn(const char *str, const char *chars) {
	const char *ptr = str;
	while (*ptr && strchr(chars, *ptr)) {
		ptr++;
	}
	return ptr - str;
}

size_t strcspn(const char *str, const char *chars) {
	const char *ptr = str;
	while (*ptr && !strchr(chars, *ptr)) {
		ptr++;
	}
	return ptr - str;
}

size_t strxfrm(char *dest, const char *src, size_t n) {
	// FIXME: Implement strxfrm properly
	size_t len = strnlen(src, n);
	if (dest != nullptr) {
		strncpy(dest, src, len);
	}
	return len;
}

int strcoll(const char *s1, const char *s2) {
	// FIXME: Implement strcoll properly
	return strcmp(s1, s2);
}