/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Defines the assert macro
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/assert.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <assert.h>
#include <stdio.h>

#ifdef __is_kernel
#include <kernel/panic.h>
#else
#include <stdlib.h>
#endif

void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function) {
	char msg[256];
	snprintf(msg, 256, "Assertion '%s' failed!\nFile: \"%s\" (line %d)\nFunction: %s",
			 assertion,
			 file,
			 line,
			 function);

#ifdef __is_kernel
	Kernel::panic(msg);
#else
	fprintf(stderr, "%s\n", msg);
	abort();
#endif
}