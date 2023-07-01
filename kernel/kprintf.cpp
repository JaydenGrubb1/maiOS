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

int kprintf(const char *__restrict__ format, ...) {
	UART comm(UART::COM1);
	int count = 0;
	bool upper = false;
	char buffer[32];

	va_list args;
	va_start(args, format);

	for (size_t i = 0; format[i] != '\0'; i++) {
		if (format[i] == '%') {
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
					int32_t d = va_arg(args, int32_t);
					itoa(d, buffer, 10);
					count += kprintf(buffer);
					break;
				}
				case 'o': {
					int32_t o = va_arg(args, int32_t);
					uitoa(o, buffer, 8);
					count += kprintf(buffer);
					break;
				}
				case 'u': {
					uint32_t u = va_arg(args, uint32_t);
					uitoa(u, buffer, 10);
					count += kprintf(buffer);
					break;
				}
				case 'X':
					upper = true;
				case 'x': {
					int32_t x = va_arg(args, int32_t);
					uitoa(x, buffer, 16);
					if (upper) {
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
					void *p = va_arg(args, void *);
					ultoa((uint64_t)p, buffer, 16);
					int len = strlen(buffer);
					int pad = 16 - len - 1;
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