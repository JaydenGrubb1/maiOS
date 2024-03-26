/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-02-03
 * @brief Random number generator for non-deterministic random numbers
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace std {
	/**
	 * @brief Random number generator for non-deterministic random numbers
	 *
	 */
	class random_device {
	  public:
		using result_type = unsigned int;

		/**
		 * @brief Construct a new random device object
		 *
		 */
		random_device(void) {}

		// explicit random_device(const std::string &token);
		// TODO implement this

		// disallow copy construction
		random_device(const random_device &) = delete;

		// disallow copy assignment
		void operator=(const random_device &) = delete;

		/**
		 * @brief Generate a random number
		 *
		 * @return The random number
		 */
		[[nodiscard]] unsigned int operator()(void);

		// double entropy(void) const;
		// TODO implement this ???

		/**
		 * @brief Returns the minimum value that can be generated
		 *
		 * @return The minimum value that can be generated
		 */
		[[nodiscard]] static constexpr unsigned int min(void) {
			return 0;
		}

		/**
		 * @brief Returns the maximum value that can be generated
		 *
		 * @return The maximum value that can be generated
		 */
		[[nodiscard]] static constexpr unsigned int max(void) {
			return static_cast<unsigned int>(-1);
		}
	};
}