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

#ifdef __is_kernel
#include <kernel/arch/uart.h>
#else
#error "Userland stdio not implemented"
#endif

#include <lib/ctype.h>
#include <lib/stdio.h>
#include <lib/stdlib.h>
#include <lib/string.h>

#define LEFT 1
#define PLUS 2
#define SPACE 4
#define PREFIX 8
#define ZEROS 16
#define UPPERCASE 32
#define SIGNED 64
#define WIDE 128

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
static size_t _vtoa(T value, char *str, int base, bool uppercase) {
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
 * @param buffer The buffer to write to, or NULL for stdout
 * @param c The character to write
 * @param pos The current position in the buffer
 * @param max_len The maximum length of the buffer
 * @return true if the buffer is full
 */
static bool _writec(char *buffer, char c, size_t *pos, size_t max_len) {
	if (*pos >= max_len - 1) {
		return true;
	}

	if (buffer) {
		buffer[*pos] = c;
	} else {
		putchar(c);
	}

	(*pos)++;
	return false;
}

/**
 * @brief Write a string to a buffer or stdout
 *
 * @param buffer The buffer to write to, or NULL for stdout
 * @param s The string to write
 * @param max_len The maximum length of the buffer
 * @param pos The current position in the buffer
 * @param max The maximum number of characters to write
 * @return true if the buffer is full
 */
static bool _writes(char *buffer, const char *s, size_t max_len, size_t *pos, size_t max) {
	for (size_t i = 0; i < max_len && s[i]; i++) {
		if (_writec(buffer, s[i], pos, max)) {
			return true;
		}
	}

	return false;
}

/**
 * @brief Write padding to a buffer or stdout
 *
 * @param buffer The buffer to write to, or NULL for stdout
 * @param c The character to pad with
 * @param max_len The maximum length of the buffer
 * @param pos The current position in the buffer
 * @param num The number of characters to write
 * @return true if the buffer is full
 */
static bool _pad(char *buffer, char c, size_t max_len, size_t *pos, int *num) {
	while ((*num)-- > 0) {
		if (_writec(buffer, c, pos, max_len)) {
			return true;
		}
	}

	return false;
}

/**
 * @brief Write a formatted string to a buffer or stdout
 *
 * @param output The buffer to write to, or NULL for stdout
 * @param max_len The maximum length of the buffer
 * @param format The format string
 * @param ap The arguments
 * @return The number of characters written
 */
static int _printf_impl(char *output, size_t max_len, const char *format, va_list ap) {
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
			if (_writec(output, format[i], &count, max_len)) {
				break;
			}
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
				precision = atoi((char *)&format[i]);
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
				[[fallthrough]];
			case 'c': {
				// TODO: support wide characters
				char c = (char)va_arg(ap, int);
				width--;
				if (!(flags & LEFT)) {
					if (_pad(output, ' ', max_len, &count, &width)) {
						break;
					}
				}
				if (_writec(output, c, &count, max_len)) {
					break;
				}
				if (_pad(output, ' ', max_len, &count, &width)) {
					break;
				}
				continue;
			}
			case 'S':
				flags |= WIDE;
				[[fallthrough]];
			case 's': {
				// TODO: support wide strings
				char *s = va_arg(ap, char *);
				int len = strnlen(s, precision);
				width -= len;
				if (!(flags & LEFT)) {
					if (_pad(output, ' ', max_len, &count, &width)) {
						break;
					}
				}
				if (_writes(output, s, len, &count, max_len)) {
					break;
				}
				if (_pad(output, ' ', max_len, &count, &width)) {
					break;
				}
				continue;
			}
			case '%': {
				if (_writec(output, '%', &count, max_len)) {
					break;
				}
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
				flags |= PREFIX | ZEROS;
				base = HEXADECIMAL;
				size = sizeof(void *);
				precision = sizeof(void *) * 2;
				break;
			}
			default: {
				if (_writec(output, format[i], &count, max_len)) {
					break;
				}
				continue;
			}
		}

		// check for buffer overrun
		if (count >= max_len - 1) [[unlikely]] {
			break;
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
					len = _vtoa((int8_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int16_t):
					len = _vtoa((int16_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int32_t):
					len = _vtoa((int32_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(int64_t):
					len = _vtoa((int64_t)value, buffer, base, flags & UPPERCASE);
					break;
			}
		} else {
			switch (size) {
				case sizeof(uint8_t):
					len = _vtoa((uint8_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint16_t):
					len = _vtoa((uint16_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint32_t):
					len = _vtoa((uint32_t)value, buffer, base, flags & UPPERCASE);
					break;
				case sizeof(uint64_t):
					len = _vtoa((uint64_t)value, buffer, base, flags & UPPERCASE);
					break;
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
			if (_pad(output, ' ', max_len, &count, &width)) {
				break;
			}
		}
		if (flags & PREFIX) {
			if (base == OCTAL) {
				if (_writec(output, '0', &count, max_len)) {
					break;
				}
			} else if (base == HEXADECIMAL) {
				if (_writec(output, '0', &count, max_len)) {
					break;
				}
				if (_writec(output, 'x' - (flags & UPPERCASE), &count, max_len)) {
					break;
				}
			}
		}
		if (value >= 0) {
			if (flags & PLUS) {
				if (_writec(output, '+', &count, max_len)) {
					break;
				}
			}
			if (flags & SPACE) {
				if (_writec(output, ' ', &count, max_len)) {
					break;
				}
			}
		} else if (flags & SIGNED) {
			if (_writec(output, '-', &count, max_len)) {
				break;
			}
		}
		if (flags & ZEROS) {
			if (_pad(output, '0', max_len, &count, &width)) {
				break;
			}
		}
		if (_pad(output, '0', max_len, &count, &precision)) {
			break;
		}
		if (_writes(output, buffer, -1, &count, max_len)) {
			break;
		}
		if (flags & LEFT) {
			if (_pad(output, ' ', max_len, &count, &width)) {
				break;
			}
		}
	}

	_writec(output, '\0', &count, -1);
	return count - 1;
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/fprintf.html
int printf(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vprintf(format, ap);
	va_end(ap);
	return count;
}

char *video = (char *)0xb8000;
int width = 80;
int height = 25;
int x = 0;
int y = 0;

bool is_printable(int c) {
	return c >= 0x20 && c <= 0x7e;
}

void clear_screen() {
	memset(video, 0, width * height * 2);
}

void move_screen_up_one_line() {
	// move screen up one line using memcpy and memset
	memmove(video, video + width * 2, (height - 1) * width * 2);
	memset(video + (height - 1) * width * 2, 0, width * 2);
	y--;
}

bool escape_sequence = false;
int sequence_code = 0;
uint8_t color = 0x0f;

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/putchar.html
int putchar(int c) {
#ifdef __is_kernel
	UART uart(UART::COM1);
	uart.write(c);

	if (c == '\033') {
		escape_sequence = true;
		sequence_code = 0;
		return c;
	}

	if (escape_sequence) {
		if (c == '[') {
			return c;
		}
		if (isdigit(c)) {
			sequence_code = sequence_code * 10 + (c - '0');
			return c;
		}
		if (c == 'J') {
			escape_sequence = false;
			clear_screen();
			return c;
		}
		if (c == 'm') {
			escape_sequence = false;

			switch (sequence_code) {
				case 40:
					color &= 0x0f;
					color |= 0x00;
					break;
				case 41:
					color &= 0x0f;
					color |= 0x40;
					break;
				case 42:
					color &= 0x0f;
					color |= 0x20;
					break;
				case 43:
					color &= 0x0f;
					color |= 0x60;
					break;
				case 44:
					color &= 0x0f;
					color |= 0x10;
					break;
				case 45:
					color &= 0x0f;
					color |= 0x50;
					break;
				case 46:
					color &= 0x0f;
					color |= 0x30;
					break;
				case 47:
					color &= 0x0f;
					color |= 0x70;
					break;
				case 30:
					color &= 0xf0;
					color |= 0x00;
					break;
				case 31:
					color &= 0xf0;
					color |= 0x04;
					break;
				case 32:
					color &= 0xf0;
					color |= 0x02;
					break;
				case 33:
					color &= 0xf0;
					color |= 0x06;
					break;
				case 34:
					color &= 0xf0;
					color |= 0x01;
					break;
				case 35:
					color &= 0xf0;
					color |= 0x05;
					break;
				case 36:
					color &= 0xf0;
					color |= 0x03;
					break;
				case 37:
					color &= 0xf0;
					color |= 0x07;
					break;
				case 0:
					color = 0x0f;
					break;

				default:
					break;
			}

			return c;
		}
	}

	if (c == '\t') {
		x += 4;
		if (x >= width) {
			x = 0;
			y++;
			if (y >= height) {
				move_screen_up_one_line();
			}
		}
		return c;
	}

	if (c == '\n') {
		x = 0;
		y++;
		if (y >= height) {
			move_screen_up_one_line();
		}
	}

	if (!is_printable(c)) {
		return NULL;
	}
	video[(y * width + x) * 2] = c;
	video[(y * width + x) * 2 + 1] = color;
	x++;
	if (x >= width) {
		x = 0;
		y++;

		if (y >= height) {
			move_screen_up_one_line();
		}
	}

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
	return _printf_impl(nullptr, -1, format, ap);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html
int vsnprintf(char *str, size_t size, const char *__restrict__ format, va_list ap) {
	return _printf_impl(str, size, format, ap);
}

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/vfprintf.html
int vsprintf(char *str, const char *__restrict__ format, va_list ap) {
	return _printf_impl(str, -1, format, ap);
}