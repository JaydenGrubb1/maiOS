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

static const char *const digits = "0123456789ABCDEF";

// Internal use only
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

// Internal use only
static size_t _int2buf(char *buffer, intmax_t value, int base, size_t size, int flags) {
	bool uppercase = flags & UPPERCASE;

	if (flags & SIGNED) {
		switch (size) {
			case sizeof(int8_t):
				return _vtoa((int8_t)value, buffer, base, uppercase);
			case sizeof(int16_t):
				return _vtoa((int16_t)value, buffer, base, uppercase);
			case sizeof(int32_t):
				return _vtoa((int32_t)value, buffer, base, uppercase);
			case sizeof(int64_t):
				return _vtoa((int64_t)value, buffer, base, uppercase);
		}
	} else {
		switch (size) {
			case sizeof(uint8_t):
				return _vtoa((uint8_t)value, buffer, base, uppercase);
			case sizeof(uint16_t):
				return _vtoa((uint16_t)value, buffer, base, uppercase);
			case sizeof(uint32_t):
				return _vtoa((uint32_t)value, buffer, base, uppercase);
			case sizeof(uint64_t):
				return _vtoa((uint64_t)value, buffer, base, uppercase);
		}
	}

	return 0;
}

// Internal use only
static bool _writec(char *buffer, char c, size_t *pos, size_t max) {
	if (*pos >= max - 1) {
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

// Internal use only
static bool _writes(char *buffer, const char *s, size_t n, size_t *pos, size_t max) {
	for (size_t i = 0; i < n && s[i]; i++) {
		if (_writec(buffer, s[i], pos, max)) {
			return true;
		}
	}

	return false;
}

// Internal use only
static int _printf_impl(char *output, size_t length, const char *format, va_list ap) {
	// TODO Make use of length
	size_t count = 0;
	char buffer[32];

	size_t size;
	int len;
	int base;
	int flags;
	int width;
	int precision;

	for (size_t i = 0; format[i] != '\0'; i++) {
		// print non-format characters
		if (format[i] != '%') {
			if (_writec(output, format[i], &count, length)) {
				goto print_end;
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
			case 'c': {
				// TODO: support wide characters
				char c = (char)va_arg(ap, int);
				if (!(flags & LEFT)) {
					while (--width > 0) {
						if (_writec(output, ' ', &count, length)) {
							goto print_end;
						}
					}
				}
				if (_writec(output, c, &count, length)) {
					goto print_end;
				}
				while (--width > 0) {
					if (_writec(output, ' ', &count, length)) {
						goto print_end;
					}
				}
				continue;
			}
			case 'S':
				flags |= WIDE;
			case 's': {
				// TODO: support wide strings
				char *s = va_arg(ap, char *);
				int len = strnlen(s, precision);
				width -= len;
				if (!(flags & LEFT)) {
					while (width-- > 0) {
						if (_writec(output, ' ', &count, length)) {
							goto print_end;
						}
					}
				}
				if (_writes(output, s, len, &count, length)) {
					goto print_end;
				}
				while (width-- > 0) {
					if (_writec(output, ' ', &count, length)) {
						goto print_end;
					}
				}
				continue;
			}
			case '%': {
				if (_writec(output, '%', &count, length)) {
					goto print_end;
				}
				continue;
			}
			case 'n': {
				int *n = va_arg(ap, int *);
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
				if (_writec(output, format[i], &count, length)) {
					goto print_end;
				}
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
			value = va_arg(ap, intmax_t);
		else
			value = va_arg(ap, int32_t);
		len = _int2buf(buffer, value, base, size, flags);

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
				if (_writec(output, ' ', &count, length)) {
					goto print_end;
				}
			}
		}
		if (flags & PREFIX) {
			if (base == OCTAL) {
				if (_writec(output, '0', &count, length)) {
					goto print_end;
				}
			} else if (base == HEXADECIMAL) {
				if (_writec(output, '0', &count, length)) {
					goto print_end;
				}
				if (_writec(output, 'x' - (flags & UPPERCASE), &count, length)) {
					goto print_end;
				}
			}
		}
		if (value >= 0) {
			if (flags & PLUS) {
				if (_writec(output, '+', &count, length)) {
					goto print_end;
				}
			}
			if (flags & SPACE) {
				if (_writec(output, ' ', &count, length)) {
					goto print_end;
				}
			}
		} else if (flags & SIGNED) {
			if (_writec(output, '-', &count, length)) {
				goto print_end;
			}
		}
		if (flags & ZEROS) {
			while (width-- > 0) {
				if (_writec(output, '0', &count, length)) {
					goto print_end;
				}
			}
		}
		while (precision-- > 0) {
			if (_writec(output, '0', &count, length)) {
				goto print_end;
			}
		}
		if (_writes(output, buffer, -1, &count, length)) {
			goto print_end;
		}
		if (flags & LEFT) {
			while (width-- > 0) {
				if (_writec(output, ' ', &count, length)) {
					goto print_end;
				}
			}
		}
	}

print_end:
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

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/putchar.html
int putchar(int c) {
#ifdef __is_kernel
	UART uart(UART::COM1);
	uart.write(c);
	return c;
#else
	// TODO Implement this
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