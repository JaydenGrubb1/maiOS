/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-30
 * @brief Memory allocation functions for the kernel
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <defines.h>
#include <stdint.h>

#include <kernel/arch/x86_64/kmalloc.h>
#include <kernel/debug.h>
#include <lib/libc/assert.h>

#define KERNEL_HEAP_SIZE (64 * MiB)

static SECTION(".heap") uint8_t heap[KERNEL_HEAP_SIZE];
static uint8_t *heap_ptr = heap;

void *kmalloc(size_t size) {
	void *ptr = heap_ptr;
	heap_ptr += size;

	if (heap_ptr > heap + KERNEL_HEAP_SIZE) {
		Debug::log_failure("Insufficient kernel heap memory");
		heap_ptr -= size;
		return nullptr;
	}

	return ptr;
}

void kfree(void *ptr, UNUSED size_t size) {
	if (!ptr) {
		return;
	}

	Debug::log_warning("kfree() is not yet implemented");
}