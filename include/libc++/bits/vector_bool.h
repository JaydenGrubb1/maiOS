/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-01
 * @brief Boolean specialization of the vector class
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/vector_base.h>

namespace kstd {
	template <typename A>
	class vector<bool, A> {
	  private:
		using value_type = uint8_t;
		using allocator_type = typename A::template rebind<value_type>::other;

		vector<value_type, allocator_type> _data;

	  public:
		constexpr vector(void) = default;

		// TODO Implement the rest of the vector interface
	};
}