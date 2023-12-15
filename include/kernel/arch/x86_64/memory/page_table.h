/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-05
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

namespace Memory::Paging {
	struct PageTableEntry {
		uint64_t value;
		bool is_present() const;
		void set_present(bool);
		bool is_writable() const;
		bool is_huge() const;
		uint64_t page_frame() const;
	};
}