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

int kprintf(const char *__restrict__ format, ...) {
	UART comm(UART::COM1);
	// TODO Implement format arguments

	int count = 0;
	char *str = (char *)format;
	while (*str != '\0') {
		comm.write(*str);
		count++;
		str++;
	}

	return count;
}