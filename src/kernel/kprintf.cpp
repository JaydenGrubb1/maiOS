/**
 * @file kernel/kprintf.cpp
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-06-30
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/kprintf.hpp>
#include <kernel/uart.hpp>
#include <lib/ctype.h>
#include <lib/stdlib.h>
#include <lib/string.h>
#include <stdarg.h>
#include <stdbool.h>

#define LEFT 1
#define PLUS 2
#define SPACE 4
#define PREFIX 8
#define ZEROS 16
#define UPPERCASE 32
#define SIGNED 64
#define WIDE 128

static const char *const digits = "0123456789ABCDEF";

/**
 * @brief Convert an integer to a string
 *
 * @tparam T The type of the integer
 * @param value The integer to convert
 * @param str The buffer to output to
 * @param base The base to convert to
 * @param uppercase Whether to use uppercase letters
 * @return The length of the string
 */
template <typename T>
static size_t vtoa(T value, char *str, int base, bool uppercase) {
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
 * @brief Convert an integer to a string and output it to a buffer
 *
 * @param buffer The buffer to output to
 * @param value The integer to convert
 * @param base The base to convert to
 * @param size The size of the integer
 * @param flags The flags to use
 * @return The length of the string
 */
static size_t int2buf(char *buffer, intmax_t value, int base, size_t size, int flags) {
	bool uppercase = flags & UPPERCASE;

	if (flags & SIGNED) {
		switch (size) {
			case sizeof(int8_t):
				return vtoa((int8_t)value, buffer, base, uppercase);
			case sizeof(int16_t):
				return vtoa((int16_t)value, buffer, base, uppercase);
			case sizeof(int32_t):
				return vtoa((int32_t)value, buffer, base, uppercase);
			case sizeof(int64_t):
				return vtoa((int64_t)value, buffer, base, uppercase);
		}
	} else {
		switch (size) {
			case sizeof(uint8_t):
				return vtoa((uint8_t)value, buffer, base, uppercase);
			case sizeof(uint16_t):
				return vtoa((uint16_t)value, buffer, base, uppercase);
			case sizeof(uint32_t):
				return vtoa((uint32_t)value, buffer, base, uppercase);
			case sizeof(uint64_t):
				return vtoa((uint64_t)value, buffer, base, uppercase);
		}
	}

	return 0;
}

int kputchar(char c) {
	UART comm(UART::COM1);
	comm.write(c);
	return c;
}

int kputs(const char *s) {
	UART comm(UART::COM1);
	int count = 0;

	for (size_t i = 0; s[i] != '\0'; i++) {
		comm.write(s[i]);
		count++;
	}

	return count;
}

int kputns(const char *s, size_t n) {
	UART comm(UART::COM1);
	int count = 0;

	for (size_t i = 0; i < n && s[i]; i++) {
		comm.write(s[i]);
		count++;
	}

	return count;
}

int kprintf(const char *__restrict__ format, ...) {
	int count = 0;
	char buffer[32];

	size_t size;
	int len;
	int base;
	int flags;
	int width;
	int precision;

	va_list args;
	va_start(args, format);

	for (size_t i = 0; format[i] != '\0'; i++) {
		// print non-format characters
		if (format[i] != '%') {
			kputchar(format[i]);
			count++;
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
			width = atoi((char *)&format[i]);
			while (isdigit(format[i])) {
				i++;
			}
		} else if (format[i] == '*') {
			i++;
			width = va_arg(args, int);
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
				precision = atoi((char *)&format[i]);
				while (isdigit(format[i])) {
					i++;
				}
			} else if (format[i] == '*') {
				i++;
				precision = va_arg(args, int);
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
				if (format[i + 1] == 'l') {
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
		switch (format[i]) {
			case 'C':
				flags |= WIDE;
			case 'c': {
				// TODO: support wide characters
				char c = (char)va_arg(args, int);
				if (!(flags & LEFT)) {
					while (--width > 0) {
						kputchar(' ');
						count++;
					}
				}
				kputchar(c);
				count++;
				while (--width > 0) {
					kputchar(' ');
					count++;
				}
				continue;
			}
			case 'S':
				flags |= WIDE;
			case 's': {
				// TODO: support wide strings
				char *s = va_arg(args, char *);
				int len = strnlen(s, precision);
				width -= len;
				if (!(flags & LEFT)) {
					while (width-- > 0) {
						kputchar(' ');
						count++;
					}
				}
				count += kputns(s, len);
				while (width-- > 0) {
					kputchar(' ');
					count++;
				}
				continue;
			}
			case '%': {
				kputchar('%');
				count++;
				continue;
			}
			case 'n': {
				int *n = va_arg(args, int *);
				*n = count;
				continue;
			}
			case 'i':
			case 'd':
				flags |= SIGNED;
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
			case 'x': {
				base = HEXADECIMAL;
				break;
			}
			case 'p': {
				flags |= PREFIX | ZEROS;
				base = HEXADECIMAL;
				size = sizeof(void *);
				precision = sizeof(void *) * 2;
				break;
			}
			default: {
				kputchar(format[i]);
				count++;
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

		// convert argument to string
		intmax_t value = 0;
		if (size > sizeof(int32_t))
			value = va_arg(args, intmax_t);
		else
			value = va_arg(args, int32_t);
		len = int2buf(buffer, value, base, size, flags);

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
			while (width-- > 0) {
				kputchar(' ');
				count++;
			}
		}
		if (flags & PREFIX) {
			if (base == OCTAL) {
				kputchar('0');
				count++;
			} else if (base == HEXADECIMAL) {
				kputchar('0');
				kputchar('x' - (flags & UPPERCASE));
				count += 2;
			}
		}
		if (value >= 0) {
			if (flags & PLUS) {
				kputchar('+');
				count++;
			}
			if (flags & SPACE) {
				kputchar(' ');
				count++;
			}
		} else if (flags & SIGNED) {
			kputchar('-');
			count++;
		}
		if (flags & ZEROS) {
			while (width-- > 0) {
				kputchar('0');
				count++;
			}
		}
		while (precision-- > 0) {
			kputchar('0');
			count++;
		}
		count += kputs(buffer);
		if (flags & LEFT) {
			while (width-- > 0) {
				kputchar(' ');
				count++;
			}
		}
	}

	va_end(args);

	return count;
}