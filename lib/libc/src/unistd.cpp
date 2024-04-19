/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-20
 * @brief UNIX standard functions and constants
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/unistd.h.html @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <errno.h>
#include <unistd.h>

#ifdef __is_kernel
#include <kernel/arch/uart.h>
#else
#error "Userland stdio not implemented"
#endif

// https://pubs.opengroup.org/onlinepubs/9699919799/functions/pwrite.html
ssize_t write(int fd, const void *buf, size_t count) {
#ifdef __is_kernel
	if (fd == 1 || fd == 2) {
		UART uart(UART::COM1);
		for (size_t i = 0; i < count; i++) {
			uart.write((reinterpret_cast<const char *>(buf))[i]);
		}
		return count;
	}
	errno = ENOTSUP;
#else
	errno = ENOSYS;
#endif
	// TODO other stuff...
	return -1;
}