/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-02-15
 * @brief Panic the kernel
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/cpu.h>
#include <kernel/debug.h>
#include <kernel/panic.h>

void Kernel::panic(std::string_view msg) {
	Debug::log_failure("Kernel panic: %s", msg.data());
	Debug::trace_stack(__builtin_frame_address(0));
	CPU::stop();
}