/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-13
 * @brief Stardard input/output functions
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdarg.h>
#include <stddef.h>

#ifdef __is_kernel
#include <arch/uart.h>
#else
#error "Userland stdio not implemented"
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define LEFT 1
#define PLUS 2
#define SPACE 4
#define PREFIX 8
#define ZEROS 16
#define UPPERCASE 32
#define SIGNED 64
#define WIDE 128
#define POINTER 256

#define BINARY 2
#define OCTAL 8
#define DECIMAL 10
#define HEXADECIMAL 16

static const char *const digits = "0123456789ABCDEF";

/**
 * @brief Convert an integer to a string
 *
 * @tparam T The type of the integer
 * @param value The integer to convert
 * @param str The string buffer to write to
 * @param base The base to convert to
 * @param uppercase Whether to use uppercase letters
 * @return The number of characters written
 */
template <typename T>
static size_t __vtoa(T value, char *str, int base, bool uppercase) {
	char lower = uppercase ? 0 : 32;
	char *ptr = str;
	size_t count = 0;
	T tmpv;

	if (value < 0) {
		value = -value;
	}

	do {
		count++;
		tmpv = value;
		value /= base;
		*ptr++ = digits[(tmpv - value * base)] | lower;
	} while (value);
	*ptr-- = '\0';

	char *tmpp = str;
	char tmpc;
	while (tmpp < ptr) {
		tmpc = *ptr;
		*ptr-- = *tmpp;
		*tmpp++ = tmpc;
	}

	return count;
}

/**
 * @brief Write a character to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param buffer The buffer to write to
 * @param buffer_len The maximum length of the buffer
 * @param c The character to write
 * @param pos The current position in the buffer
 */
static void __writec(bool use_buffer, char *buffer, size_t buffer_len, char c, size_t *pos) {
	if (*pos >= buffer_len - 1) {
		(*pos)++;
		return;
	}

	if (use_buffer) {
		if (buffer) {
			buffer[*pos] = c;
		}
	} else {
		putchar(c);
	}

	(*pos)++;
}

/**
 * @brief Write a wide character to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param buffer The buffer to write to
 * @param buffer_len The maximum length of the buffer
 * @param c The character to write
 * @param pos The current position in the buffer
 */
static void __writewc(bool use_buffer, char *buffer, size_t buffer_len, wchar_t c, size_t *pos) {
	char mb[MB_CUR_MAX];
	size_t len = wctomb(mb, c);

	if (*pos >= buffer_len - len) {
		(*pos) += len;
		return;
	}

	for (size_t i = 0; i < len; i++) {
		__writec(use_buffer, buffer, buffer_len, mb[i], pos);
	}
}

/**
 * @brief Write a string to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param buffer The buffer to write to
 * @param buffer_len The maximum length of the buffer
 * @param str The string to write
 * @param str_len The length of the string
 * @param pos The current position in the buffer
 */
static void __writes(bool use_buffer, char *buffer, size_t buffer_len, const char *str, size_t str_len, size_t *pos) {
	for (size_t i = 0; i < str_len && str[i] != '\0'; i++) {
		__writec(use_buffer, buffer, buffer_len, str[i], pos);
	}
}

/**
 * @brief Write a wide string to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param buffer The buffer to write to
 * @param buffer_len The maximum length of the buffer
 * @param str The string to write
 * @param str_len The length of the string
 * @param pos The current position in the buffer
 */
static void __writews(bool use_buffer, char *buffer, size_t buffer_len, const wchar_t *str, size_t str_len, size_t *pos) {
	for (size_t i = 0; i < str_len && str[i] != '\0'; i++) {
		__writewc(use_buffer, buffer, buffer_len, str[i], pos);
	}
}

/**
 * @brief Write padding to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param buffer The buffer to write to
 * @param c The character to pad with
 * @param max_len The maximum length of the buffer
 * @param pos The current position in the buffer
 * @param num The number of characters to write
 * @return true if the buffer is full
 */
static void __pad(bool use_buffer, char *buffer, char c, size_t max_len, size_t *pos, int *num) {
	while ((*num)-- > 0) {
		__writec(use_buffer, buffer, max_len, c, pos);
	}
}

/**
 * @brief Write a formatted string to a buffer or stdout
 *
 * @param use_buffer Whether to write to a buffer or stdout
 * @param output The buffer to write to
 * @param max_len The maximum length of the buffer
 * @param format The format string
 * @param ap The arguments
 * @return The number of characters written
 */
static int __printf_impl(bool use_buffer, char *output, size_t max_len, const char *format, va_list ap) {
	char buffer[32];
	size_t count = 0;

	size_t size;
	int len;
	int base;
	int flags;
	int width;
	int precision;

	for (size_t i = 0; format[i] != '\0'; i++) {
		// print non-format characters
		if (format[i] != '%') {
			__writec(use_buffer, output, max_len, format[i], &count);
			continue;
		}
		i++;

		// flags
		flags = 0;
		while (true) {
			switch (format[i++]) {
				case '-':
					flags |= LEFT;
					continue;
				case '+':
					flags |= PLUS;
					continue;
				case ' ':
					flags |= SPACE;
					continue;
				case '#':
					flags |= PREFIX;
					continue;
				case '0':
					flags |= ZEROS;
					continue;
				default:
					i--;
					break; // end of flags
			}
			break; // break loop
		}

		// field width
		width = -1;
		if (isdigit(format[i])) {
			width = atoi(&format[i]);
			while (isdigit(format[i])) {
				i++;
			}
		} else if (format[i] == '*') {
			i++;
			width = va_arg(ap, int);
			if (width < 0) {
				flags |= LEFT;
				width = -width;
			}
		}

		// precision
		precision = -1;
		if (format[i] == '.') {
			i++;
			if (isdigit(format[i])) {
				precision = atoi(&format[i]);
				while (isdigit(format[i])) {
					i++;
				}
			} else if (format[i] == '*') {
				i++;
				precision = va_arg(ap, int);
			}
		}

		// size modifiers
		switch (format[i++]) {
			case 'h':
				size = sizeof(short);
				if (format[i] == 'h') {
					size = sizeof(char);
					i++;
				}
				break;
			case 'l':
				size = sizeof(long);
				flags |= WIDE;
				if (format[i] == 'l') {
					size = sizeof(long long);
					i++;
				}
				break;
			case 'j':
				size = sizeof(intmax_t);
				break;
			case 'z':
				size = sizeof(size_t);
				break;
			case 't':
				size = sizeof(ptrdiff_t);
				break;
			default:
				size = sizeof(int);
				i--;
				break;
		}

		// conversion specifiers
		base = DECIMAL;
		switch (format[i]) {
			case 'C':
				flags |= WIDE;
				[[fallthrough]];
			case 'c': {
				if (flags & WIDE) {
					wchar_t c = (wchar_t)va_arg(ap, int);
					width--;
					if (!(flags & LEFT)) {
						__pad(use_buffer, output, ' ', max_len, &count, &width);
					}
					__writewc(use_buffer, output, max_len, c, &count);
				} else {
					char c = (char)va_arg(ap, int);
					width--;
					if (!(flags & LEFT)) {
						__pad(use_buffer, output, ' ', max_len, &count, &width);
					}
					__writec(use_buffer, output, max_len, c, &count);
				}
				__pad(use_buffer, output, ' ', max_len, &count, &width);
				continue;
			}
			case 'S':
				flags |= WIDE;
				[[fallthrough]];
			case 's': {
				if (flags & WIDE) {
					wchar_t *s = va_arg(ap, wchar_t *);
					len = wcsnlen(s, precision); // TODO get byte length (spec) instead ???
					width -= len;
					if (!(flags & LEFT)) {
						__pad(use_buffer, output, ' ', max_len, &count, &width);
					}
					__writews(use_buffer, output, max_len, s, len, &count);
				} else {
					char *s = va_arg(ap, char *);
					len = strnlen(s, precision);
					width -= len;
					if (!(flags & LEFT)) {
						__pad(use_buffer, output, ' ', max_len, &count, &width);
					}
					__writes(use_buffer, output, max_len, s, len, &count);
				}
				__pad(use_buffer, output, ' ', max_len, &count, &width);
				continue;
			}
			case '%': {
				__writec(use_buffer, output, max_len, '%', &count);
				continue;
			}
			case 'n': {
				int *n = va_arg(ap, int *);
				*n = count;
				continue;
			}
			case 'i':
				[[fallthrough]];
			case 'd':
				flags |= SIGNED;
				[[fallthrough]];
			case 'u': {
				base = DECIMAL;
				break;
			}
			case 'o': {
				base = OCTAL;
				break;
			}
			case 'X':
				flags |= UPPERCASE;
				[[fallthrough]];
			case 'x': {
				base = HEXADECIMAL;
				break;
			}
			case 'p': {
				flags |= PREFIX | ZEROS | POINTER;
				base = HEXADECIMAL;
				size = sizeof(void *);
				precision = sizeof(void *) * 2;
				break;
			}
			default: {
				__writec(use_buffer, output, max_len, format[i], &count);
				continue;
			}
		}

		// fix flags
		if (!(flags & SIGNED)) {
			flags &= ~(PLUS | SPACE);
		}
		if (flags & LEFT) {
			flags &= ~ZEROS;
		}
		if (flags & PLUS) {
			flags &= ~SPACE;
		}
		if (precision >= 0) {
			flags &= ~ZEROS;
		}

		// get argument to convert
		intmax_t value = 0;
		if (size > sizeof(int32_t))
			value = va_arg(ap, intmax_t);
		else
			value = va_arg(ap, int32_t);

		// convert argument to string
		if (flags & SIGNED) {
			switch (size) {
				case sizeof(int8_t):
					len = __vtoa(static_cast<int8_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int16_t):
					len = __vtoa(static_cast<int16_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int32_t):
					len = __vtoa(static_cast<int32_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int64_t):
					len = __vtoa(static_cast<int64_t>(value), buffer, base, flags & UPPERCASE);
					break;
			}
		} else {
			switch (size) {
				case sizeof(uint8_t):
					len = __vtoa(static_cast<uint8_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint16_t):
					len = __vtoa(static_cast<uint16_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint32_t):
					len = __vtoa(static_cast<uint32_t>(value), buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint64_t):
					len = __vtoa(static_cast<uint64_t>(value), buffer, base, flags & UPPERCASE);
					break;
			}
		}

		// check if arguement is nullptr
		if (flags & POINTER && value == 0) {
			memcpy(buffer, "(nullptr)\0", 10);
			len = 9;
			precision = -1;
			flags &= ~(PREFIX | ZEROS);
		}

		// decrease width by buffer and prefix length
		if (flags & PREFIX) {
			if (base == OCTAL) {
				width--;
			} else if (base == HEXADECIMAL) {
				width -= 2;
			}
		}
		if (value < 0 || (flags & (PLUS | SPACE))) {
			width--;
		}

		// decrease width and precision by buffer length
		if (precision > len) {
			width -= precision;
			precision -= len;
		} else {
			precision = 0;
			width -= len;
		}

		// output formatted string
		if (!(flags & LEFT) && !(flags & ZEROS)) {
			__pad(use_buffer, output, ' ', max_len, &count, &width);
		}
		if (flags & PREFIX) {
			if (base == OCTAL) {
				__writec(use_buffer, output, max_len, '0', &count);
			} else if (base == HEXADECIMAL) {
				__writec(use_buffer, output, max_len, '0', &count);
				__writec(use_buffer, output, max_len, 'x' - (flags & UPPERCASE), &count);
			}
		}
		if (value >= 0) {
			if (flags & PLUS) {
				__writec(use_buffer, output, max_len, '+', &count);
			}
			if (flags & SPACE) {
				__writec(use_buffer, output, max_len, ' ', &count);
			}
		} else if (flags & SIGNED) {
			__writec(use_buffer, output, max_len, '-', &count);
		}
		if (flags & ZEROS) {
			__pad(use_buffer, output, '0', max_len, &count, &width);
		}
		__pad(use_buffer, output, '0', max_len, &count, &precision);
		__writes(use_buffer, output, max_len, buffer, -1, &count);
		if (flags & LEFT) {
			__pad(use_buffer, output, ' ', max_len, &count, &width);
		}
	}

	__writec(use_buffer, output, -1, '\0', &count);
	return count - 1;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/putchar.html
int putchar(int c) {
#ifdef __is_kernel
	static UART uart(UART::COM1);
	uart.write(c);
	return c;
#else
	// TODO: implement this
	return EOF;
#endif
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/puts.html
int puts(const char *s) {
	int count = 0;

	for (size_t i = 0; s[i] != '\0'; i++) {
		putchar(s[i]);
		count++;
	}

	return count;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html
int printf(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vprintf(format, ap);
	va_end(ap);
	return count;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html
int snprintf(char *str, size_t size, const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vsnprintf(str, size, format, ap);
	va_end(ap);
	return count;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html
int sprintf(char *str, const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vsprintf(str, format, ap);
	va_end(ap);
	return count;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html
int vprintf(const char *__restrict__ format, va_list ap) {
	return __printf_impl(false, nullptr, -1, format, ap);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html
int vsnprintf(char *str, size_t size, const char *__restrict__ format, va_list ap) {
	return __printf_impl(true, str, size, format, ap);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html
int vsprintf(char *str, const char *__restrict__ format, va_list ap) {
	return __printf_impl(true, str, -1, format, ap);
}