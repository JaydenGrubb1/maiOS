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

#include <stdint.h>

/**
 * @brief Main entry point for the operating (64-bit)
 * @param magic The magic number passed by multiboot2
 * @param addr The address of the multiboot2 info structure
 */
extern "C" void kmain(uint32_t magic, uint8_t *addr)
{
	// TODO
}