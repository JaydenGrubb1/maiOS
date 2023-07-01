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
#include <lib/stdlib.h>
#include <lib/string.h>
#include <stdarg.h>
#include <stdbool.h>

/**
 * @brief Convert the next argument to a string and write it to the buffer
 *
 * @param buffer The buffer to write to
 * @param args The argument list
 * @param base The base to use for the conversion
 * @param size The size of the argument
 * @param is_signed Whether the argument is signed or not
 */
void args2buf(char *buffer, va_list *args, int base, size_t size, bool is_signed) {
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

int kprintf(const char *__restrict__ format, ...) {
	UART comm(UART::COM1);
	int count = 0;
	bool is_upper = false;
	char buffer[32];
	size_t size;

	va_list args;
	va_start(args, format);

	for (size_t i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
			// size modifiers
			switch (format[i + 1]) {
				case 'h':
					if (format[i + 2] == 'h') {
						size = sizeof(char);
						i += 2;
					} else {
						size = sizeof(short);
						i++;
					}
					break;
				case 'l':
					if (format[i + 2] == 'l') {
						size = sizeof(long long);
						i += 2;
					} else {
						size = sizeof(long);
						i++;
					}
					break;
				case 'j':
					size = sizeof(intmax_t);
					i++;
					break;
				case 'z':
					size = sizeof(size_t);
					i++;
					break;
				case 't':
					size = sizeof(ptrdiff_t);
					i++;
					break;
				default:
					size = sizeof(int);
					break;
			}

			// conversion specifiers
			switch (format[++i]) {
				case 'c': {
					char c = (char)va_arg(args, int);
					comm.write(c);
					count++;
					break;
				}
				case 's': {
					char *s = va_arg(args, char *);
					count += kprintf(s);
					break;
				}
				case 'i':
				case 'd': {
					args2buf(buffer, &args, DECIMAL, size, true);
					count += kprintf(buffer);
					break;
				}
				case 'o': {
					args2buf(buffer, &args, OCTAL, size, false);
					count += kprintf(buffer);
					break;
				}
				case 'u': {
					args2buf(buffer, &args, DECIMAL, size, false);
					count += kprintf(buffer);
					break;
				}
				case 'X':
					is_upper = true;
				case 'x': {
					args2buf(buffer, &args, HEXADECIMAL, size, false);
					if (is_upper) {
						for (size_t i = 0; buffer[i] != '\0'; i++) {
							if (buffer[i] >= 'a' && buffer[i] <= 'z') {
								buffer[i] -= 32;
							}
						}
					}
					count += kprintf(buffer);
					break;
				}
				case 'p': {
					args2buf(buffer, &args, HEXADECIMAL, sizeof(void *), false);
					int len = strlen(buffer);
					int pad = (sizeof(void *) * 2) - len - 1;
					memmove(buffer + pad, buffer, len);
					memset(buffer, '0', pad);
					count += kprintf("0x");
					count += kprintf(buffer);
					break;
				}
				case 'n': {
					int *n = va_arg(args, int *);
					*n = count;
					break;
				}
				case '%': {
					comm.write('%');
					count++;
					break;
				}
				default: {
					comm.write(format[i]);
					count++;
					break;
				}
			}
		} else {
			comm.write(format[i]);
			count++;
		}
	}

	va_end(args);

	return count;
}