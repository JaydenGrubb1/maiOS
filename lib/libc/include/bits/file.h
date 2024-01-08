/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-08
 * @brief // DOC
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

typedef struct {
	int _fd;
	int _flags;
	char *_read_base;
	char *_read_end;
	char *_read_ptr;
	char *_write_base;
	char *_write_end;
	char *_write_ptr;
} FILE;