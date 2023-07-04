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

/**
 * @brief Convert the next argument to a string and write it to the buffer
 *
 * @param buffer The buffer to write to
 * @param args The argument list
 * @param base The base to use for the conversion
 * @param size The size of the argument
 * @param is_signed Whether the argument is signed or not
 */
static void args2buf(char *buffer, va_list *args, int base, size_t size, bool is_signed) {
	if (is_signed) {
		switch (size) {
			case sizeof(int8_t):
				btoa(va_arg(*args, int32_t), buffer, base);
				break;
			case sizeof(int16_t):
				stoa(va_arg(*args, int32_t), buffer, base);
				break;
			case sizeof(int32_t):
				itoa(va_arg(*args, int32_t), buffer, base);
				break;
			case sizeof(int64_t):
				ltoa(va_arg(*args, int64_t), buffer, base);
				break;
		}
	} else {
		switch (size) {
			case sizeof(uint8_t):
				ubtoa(va_arg(*args, uint32_t), buffer, base);
				break;
			case sizeof(uint16_t):
				ustoa(va_arg(*args, uint32_t), buffer, base);
				break;
			case sizeof(uint32_t):
				uitoa(va_arg(*args, uint32_t), buffer, base);
				break;
			case sizeof(uint64_t):
				ultoa(va_arg(*args, uint64_t), buffer, base);
				break;
		}
	}
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
			case 'd': {
				base = DECIMAL;
				flags |= SIGNED;
				break;
			}
			case 'o': {
				base = OCTAL;
				break;
			}
			case 'u': {
				base = DECIMAL;
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
		args2buf(buffer, &args, base, size, flags & SIGNED);
		bool positive = buffer[0] != '-';
		if (flags & UPPERCASE) {
			for (size_t j = 0; buffer[j] != '\0'; j++) {
				if (buffer[j] >= 'a' && buffer[j] <= 'z') {
					buffer[j] -= 32;
				}
			}
		}

		// decrease width by buffer and prefix length
		if (flags & PREFIX) {
			if (base == OCTAL) {
				width--;
			} else if (base == HEXADECIMAL) {
				width -= 2;
			}
		}
		if (positive && (flags & (PLUS | SPACE))) {
			width--;
		}

		// decrease width and precision by buffer length
		int len = strlen(buffer);
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
		if (positive && (flags & PLUS)) {
			kputchar('+');
			count++;
		}
		if (positive && (flags & SPACE)) {
			kputchar(' ');
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