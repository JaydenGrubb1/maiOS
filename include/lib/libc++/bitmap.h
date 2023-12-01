/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-01
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <lib/libc++/bits/allocator.h>
#include <lib/libc++/vector.h>

namespace kstd {
	template <typename Alloc = allocator<bool>>
	class bitmap {
	  private:
		using storage_type = uint8_t;
		using allocator_type = typename Alloc::template rebind<storage_type>::other;

		vector<storage_type, allocator_type> _data;

	  public:
		constexpr bitmap(void) = default;
	};
}