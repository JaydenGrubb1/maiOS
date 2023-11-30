/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-30
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdint.h>

#include <kernel/arch/x86_64/kmalloc.h>

__attribute__((section(".heap"))) static uint8_t heap[KERNEL_HEAP_SIZE];

/*
 * Very simple temporary heap implementation
 */

static uint8_t *heap_ptr = heap;

void *kmalloc(size_t size) {
	void *ptr = heap_ptr;
	heap_ptr += size;
	return ptr;
}

void kfree(void *ptr) {
	(void)ptr;
}