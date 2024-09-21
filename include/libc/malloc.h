/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-09-22
 * @brief Extended C library memory allocation functions
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/malloc.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define M_ARENA_MAX 1
#define M_ARENA_TEST 2
#define M_CHECK_ACTION 3
#define M_MMAP_MAX 4
#define M_MMAP_THRESHOLD 5
#define M_MXFAST 6
#define M_PERTURB 7
#define M_TOP_PAD 8
#define M_TRIM_THRESHOLD 9

struct mallinfo {
	size_t arena;
	size_t ordblks;
	size_t smblks;
	size_t hblks;
	size_t hblkhd;
	size_t usmblks;
	size_t fsmblks;
	size_t uordblks;
	size_t fordblks;
	size_t keepcost;
};

struct mallinfo mallinfo(void);

int malloc_info(int options, FILE *stream);

void malloc_stats(void);

int mallopt(int param, int value);

int malloc_trim(size_t pad);

size_t malloc_usable_size(void *ptr);

#ifdef __cplusplus
}
#endif