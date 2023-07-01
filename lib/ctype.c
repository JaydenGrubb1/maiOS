/**
 * @file lib/ctype.c
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-01
 * @brief Character type functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <lib/ctype.h>

int tolower(int c) {
	// FIXME Does not handle non-ASCII characters or locales
	if (c >= 'A' && c <= 'Z') {
		return c + 32;
	} else {
		return c;
	}
}

int toupper(int c) {
	// FIXME Does not handle non-ASCII characters or locales
	if (c >= 'a' && c <= 'z') {
		return c - 32;
	} else {
		return c;
	}
}