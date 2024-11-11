/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-11-11
 * @brief Contains definitions for the Global Descriptor Table (GDT)
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define GDT_KCODE 0x08
#define GDT_KDATA 0x10
#define GDT_UCODE 0x18
#define GDT_UDATA 0x20
#define GDT_TSS 0x28