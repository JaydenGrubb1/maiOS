/**
 * @file kernel/kmain.cpp
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-20
 * @brief Main entry point for the operating system (64-bit)
 * 
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 * 
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot2.h>
#include <kernel/uart.h>

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, uint32_t addr)
{
	bool success = Multiboot2::init((uint8_t *)addr);

	UART comm(UART::COM1);

	char *str = "COM1:\n> HelloWorld!\n";
	while (*str != '\0')
	{
		comm.write(*str);
		str++;
	}

	return;
}