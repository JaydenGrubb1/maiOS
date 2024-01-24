/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Various debugging functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <kernel/defines.h>

#define DEFAULT_MAX_FRAMES 32

namespace Debug {
	/**
	 * @brief Log a message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log a failure message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_failure(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log an info message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_info(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log an ok message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_ok(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log a test message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_test(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log a warning message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_warning(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Log a raw message to output
	 *
	 * @param format The format string
	 * @param ... The format arguments
	 */
	void log_raw(const char *format, ...) FORMAT(printf, 1, 2);

	/**
	 * @brief Print a stack trace to output
	 *
	 */
	void trace_stack(void);

	/**
	 * @brief Print a stack trace to output
	 *
	 * @param frame_ptr The frame pointer to start from
	 */
	void trace_stack(void *frame_ptr);
}