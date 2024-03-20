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

#include <bits/mbstate.h>

struct _IO_FILE {
	int _fd;
	int _flags;
	char *_read_base;
	char *_read_end;
	char *_read_ptr;
	char *_write_base;
	char *_write_end;
	char *_write_ptr;
	mbstate_t _mbstate;
	// TODO _offset
	// TODO _mutex
};

typedef struct _IO_FILE FILE;

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2
#define _IOEOF 4
#define _IOERR 8