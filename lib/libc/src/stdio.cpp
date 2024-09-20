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
#include <stdint.h>

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

#define _STRBUF -1
#define _STDIN 0
#define _STDOUT 1
#define _STDERR 2

static const char *const _digits = "0123456789ABCDEF";

static FILE _stdin;
static FILE _stdout;
static FILE _stderr;

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

#ifdef __is_kernel
static char _stdin_buffer[BUFSIZ];
static char _stdout_buffer[BUFSIZ];
#endif

extern "C" void __stdio_init(void) {
	_stdin._fd = _STDIN;
	_stdout._fd = _STDOUT;
	_stderr._fd = _STDERR;

	_stdin._flags = _IOFBF;
	_stdout._flags = _IOLBF;
	_stderr._flags = _IONBF;

#ifdef __is_kernel
	_stdin._read_base = _stdin_buffer;
	_stdin._read_end = _stdin_buffer + BUFSIZ;
	_stdin._read_ptr = _stdin_buffer;
	_stdin._write_base = _stdin_buffer;
	_stdin._write_end = _stdin_buffer + BUFSIZ;
	_stdin._write_ptr = _stdin_buffer;

	_stdout._read_base = _stdout_buffer;
	_stdout._read_end = _stdout_buffer + BUFSIZ;
	_stdout._read_ptr = _stdout_buffer;
	_stdout._write_base = _stdout_buffer;
	_stdout._write_end = _stdout_buffer + BUFSIZ;
	_stdout._write_ptr = _stdout_buffer;

	_stderr._read_base = nullptr;
	_stderr._read_end = nullptr;
	_stderr._read_ptr = nullptr;
	_stderr._write_base = nullptr;
	_stderr._write_end = nullptr;
	_stderr._write_ptr = nullptr;
#else
#error "Userland stdio not implemented"
#endif
}

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
		*ptr++ = _digits[(tmpv - value * base)] | lower;
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
 * @brief Pad a file stream with a character
 *
 * @param stream The file stream to pad
 * @param c The character to pad with
 * @param num The number of characters to pad
 * @return The number of characters written, or EOF on failure
 */
static int __pad(FILE *stream, char c, int num) {
	int i;
	for (i = 0; i < num; i++) {
		if (fputc(c, stream) == EOF) {
			return EOF;
		}
	}
	return i;
}

/**
 * @brief Write a wide character to a file stream
 *
 * @param c The wide character to write
 * @param stream The file stream to write to
 * @return The number of bytes written, or EOF on failure
 */
static int __fputwc(wchar_t c, FILE *stream) {
	char mb[MB_CUR_MAX];
	int len = wcrtomb(mb, c, &stream->_mbstate);
	if (len == -1) {
		stream->_flags |= _IOERR;
		// propagate errno from wctomb
		return EOF;
	}
	if (fileno(stream) == _STRBUF) {
		if (stream->_write_end - stream->_write_ptr < len) {
			stream->_flags |= _IOEOF;
			return len;
		}
	}
	int count = fwrite(mb, sizeof(char), len, stream);
	if (count == len) {
		return len;
	} else {
		// propagate errno from fwrite
		return EOF;
	}
}

size_t fwrite(const void *ptr, size_t size, size_t num, FILE *stream) {
	size_t len = size * num;
	if (len == 0) [[unlikely]] {
		return 0;
	}
	if (stream->_flags & _IONBF) {
		if (write(fileno(stream), ptr, len) < 0) {
			stream->_flags |= _IOERR;
			// propagate errno from write
			return 0;
		}
		return len;
	}
	if (!stream->_write_ptr) [[unlikely]] {
		if (fileno(stream) == _STRBUF) {
			return len;
		} else {
			return 0;
		}
	}

	size_t count = 0;
	const char *buffer = reinterpret_cast<const char *>(ptr);

	while (len > 0) {
		if (stream->_write_ptr >= stream->_write_end) [[unlikely]] {
			if (fileno(stream) == _STRBUF) {
				stream->_flags |= _IOEOF;
				return len;
			}

			if (fflush(stream) == EOF) {
				// propagate errno from fflush
				return count / size;
			}
		}

		if (!feof(stream)) [[likely]] {
			*stream->_write_ptr++ = *buffer;
		}
		if ((stream->_flags & _IOLBF) && *buffer == '\n') {
			if (fflush(stream) == EOF) {
				// propagate errno from fflush
				return count / size;
			}
		}

		buffer++;
		count++;
		len--;
	}

	return count / size;
}

int fflush(FILE *stream) {
	if (!stream) {
		// TODO flush all streams
		errno = ENOTSUP;
		return EOF;
	}
	if (fileno(stream) == _STRBUF || stream->_flags & _IONBF) {
		return 0;
	}
	if (!stream->_write_ptr) {
		stream->_flags |= _IOERR;
		errno = EBADF;
		return EOF;
	}

	if (write(fileno(stream), stream->_write_base, stream->_write_ptr - stream->_write_base) < 0) {
		stream->_flags |= _IOERR;
		// propagate errno from write
		return EOF;
	}

	stream->_write_ptr = stream->_write_base;
	return 0;
}

int feof(FILE *stream) {
	return stream->_flags & _IOEOF;
}

int ferror(FILE *stream) {
	return stream->_flags & _IOERR;
}

void clearerr(FILE *stream) {
	stream->_flags &= ~(_IOEOF | _IOERR);
}

int fileno(FILE *stream) {
	return stream->_fd;
}

int fputc(int c, FILE *stream) {
	char byte = c;
	size_t count = fwrite(&byte, sizeof(char), 1, stream);
	if (count == 1) {
		return byte;
	} else {
		return EOF;
	}
}

int fputs(const char *s, FILE *stream) {
	size_t size = strlen(s);
	size_t count = fwrite(s, sizeof(char), size, stream);
	if (count == size) {
		return count;
	} else {
		return EOF;
	}
}

int putchar(int c) {
	return fputc(c, stdout);
}

int puts(const char *s) {
	int count = fputs(s, stdout);
	if (count == EOF || fputc('\n', stdout) == EOF) {
		return EOF;
	}
	return count + 1;
}

int printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vprintf(format, ap);
	va_end(ap);
	return count;
}

int snprintf(char *str, size_t size, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vsnprintf(str, size, format, ap);
	va_end(ap);
	return count;
}

int sprintf(char *str, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vsprintf(str, format, ap);
	va_end(ap);
	return count;
}

int fprintf(FILE *stream, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int count = vfprintf(stream, format, ap);
	va_end(ap);
	return count;
}

int vprintf(const char *format, va_list ap) {
	return vfprintf(stdout, format, ap);
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
	if (!str || size == 0) {
		str = nullptr;
		size = 0;
	}

	FILE stream;
	stream._fd = _STRBUF;
	stream._flags = _IOFBF;
	stream._write_base = str;
	stream._write_end = str + size - 1UL;
	stream._write_ptr = str;

	int count = vfprintf(&stream, format, ap);
	if (stream._write_ptr) {
		*stream._write_ptr = '\0';
	}
	return count;
}

int vsprintf(char *str, const char *format, va_list ap) {
	FILE stream;
	stream._fd = _STRBUF;
	stream._flags = _IOFBF;
	stream._write_base = str;
	stream._write_end = reinterpret_cast<char *>(-1UL);
	stream._write_ptr = str;

	int count = vfprintf(&stream, format, ap);
	if (stream._write_ptr) {
		*stream._write_ptr = '\0';
	}
	return count;
}

int vfprintf(FILE *stream, const char *format, va_list ap) {
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
		if (format[i] != '%') [[likely]] {
			count += fputc(format[i], stream) != EOF;
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
						count += __pad(stream, ' ', width);
					}
					count += __fputwc(c, stream);
				} else {
					char c = (char)va_arg(ap, int);
					width--;
					if (!(flags & LEFT)) {
						count += __pad(stream, ' ', width);
					}
					count += fputc(c, stream) != EOF;
				}
				if (flags & LEFT) {
					count += __pad(stream, ' ', width);
				}
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
						count += __pad(stream, ' ', width);
					}
					for (int i = 0; i < len; i++) {
						count += __fputwc(s[i], stream);
					}
				} else {
					char *s = va_arg(ap, char *);
					len = strnlen(s, precision);
					width -= len;
					if (!(flags & LEFT)) {
						count += __pad(stream, ' ', width);
					}
					for (int i = 0; i < len; i++) {
						count += fputc(s[i], stream) != EOF;
					}
				}
				if (flags & LEFT) {
					count += __pad(stream, ' ', width);
				}
				continue;
			}
			case '%': {
				count += fputc('%', stream) != EOF;
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
				count += fputc(format[i], stream) != EOF;
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
		if (size > sizeof(int32_t)) {
			value = va_arg(ap, intmax_t);
		} else {
			value = va_arg(ap, int32_t);
		}

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
			count += __pad(stream, ' ', width);
		}
		if (flags & PREFIX) {
			if (base == OCTAL) {
				count += fputc('0', stream) != EOF;
			} else if (base == HEXADECIMAL) {
				count += fputc('0', stream) != EOF;
				count += fputc('x' - (flags & UPPERCASE), stream) != EOF;
			}
		}
		if (value >= 0) {
			if (flags & PLUS) {
				count += fputc('+', stream) != EOF;
			}
			if (flags & SPACE) {
				count += fputc(' ', stream) != EOF;
			}
		} else if (flags & SIGNED) {
			count += fputc('-', stream) != EOF;
		}
		if (flags & ZEROS) {
			count += __pad(stream, '0', width);
		}
		count += __pad(stream, '0', precision);
		for (int i = 0; i < len; i++) {
			count += fputc(buffer[i], stream) != EOF;
		}
		if (flags & LEFT) {
			count += __pad(stream, ' ', width);
		}
	}

	if (ferror(stream)) {
		return EOF;
	} else {
		return count;
	}
}