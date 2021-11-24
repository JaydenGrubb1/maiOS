/**
 * @file kernel/io.cpp
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-24
 * @brief Used to read and write value to IO
 * 
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 * 
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdint.h>
#include <kernel/io.h>

/**
 * @brief Reads an 8-bit value from a port
 * @param port The port to read from
 * @return An 8-bit value
 */
uint8_t IO::in8(uint16_t port)
{
	uint8_t value;
	asm volatile("inb %1, %0"
				 : "=a"(value)
				 : "Nd"(port));
	return value;
}

/**
 * @brief Reads a 16-bit value from a port
 * @param port The port to read from
 * @return A 16-bit value
 */
uint16_t IO::in16(uint16_t port)
{
	uint16_t value;
	asm volatile("inw %1, %0"
				 : "=a"(value)
				 : "Nd"(port));
	return value;
}

/**
 * @brief Reads a 32-bit value from a port
 * @param port The port to read from
 * @return A 32-bit value
 */
uint32_t IO::in32(uint16_t port)
{
	uint32_t value;
	asm volatile("inl %1, %0"
				 : "=a"(value)
				 : "Nd"(port));
	return value;
}

/**
 * @brief Writes an 8-bit value to a port
 * @param port The port to write to
 * @param value The value to write to the port
 */
void IO::out8(uint16_t port, uint8_t value)
{
	asm volatile("outb %0, %1" ::"a"(value), "Nd"(port));
}

/**
 * @brief Writes a 16-bit value to a port
 * @param port The port to write to
 * @param value The value to write to the port
 */
void IO::out16(uint16_t port, uint16_t value)
{
	asm volatile("outw %0, %1" ::"a"(value), "Nd"(port));
}

/**
 * @brief Writes a 32-bit value to a port
 * @param port The port to write to
 * @param value The value to write to the port
 */
void IO::out32(uint16_t port, uint32_t value)
{
	asm volatile("outl %0, %1" ::"a"(value), "Nd"(port));
}