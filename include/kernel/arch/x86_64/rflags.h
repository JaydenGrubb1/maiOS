/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-11-07
 * @brief Contains definitions for the RFLAGS register
 * 
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 * 
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define RFLAGS_CARRY (1 << 0)
#define RFLAGS_RESERVED (1 << 1)
#define RFLAGS_PARITY (1 << 2)
#define RFLAGS_AUX_CARRY (1 << 4)
#define RFLAGS_ZERO (1 << 6)
#define RFLAGS_SIGN (1 << 7)
#define RFLAGS_TRAP (1 << 8)
#define RFLAGS_INTERRUPT_ENABLE (1 << 9)
#define RFLAGS_DIRECTION (1 << 10)
#define RFLAGS_OVERFLOW (1 << 11)
#define RFLAGS_IO_PRIVILEGE_LEVEL_MASK (3 << 12)
#define RFLAGS_NESTED_TASK (1 << 14)
#define RFLAGS_RESUME (1 << 16)
#define RFLAGS_VIRTUAL8086_MODE (1 << 17)
#define RFLAGS_ALIGNMENT_CHECK (1 << 18)
#define RFLAGS_VIRTUAL_INTERRUPT (1 << 19)
#define RFLAGS_VIRTUAL_INTERRUPT_PENDING (1 << 20)
#define RFLAGS_ID (1 << 21)