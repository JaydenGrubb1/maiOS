/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-02-03
 * @brief Random number generator using a linear congruential generator
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>
#include <type_traits>

namespace std {
	/**
	 * @brief Random number generator using a linear congruential engine
	 *
	 * @tparam T The type of the random number
	 * @tparam a The multiplier value
	 * @tparam c The increment value
	 * @tparam m The modulus value
	 */
	template <typename T, T a, T c, T m>
	class linear_congruential_engine {
		static_assert(std::is_integral_v<T>, "T must be an integral type");
		static_assert(std::is_unsigned_v<T>, "T must be an unsigned type");
		static_assert(m == 0 || (a < m && c < m), "Invalid parameters for linear_congruential_engine");

	  public:
		/**
		 * @brief The multiplier value
		 *
		 */
		static constexpr T multiplier = a;

		/**
		 * @brief The increment value
		 *
		 */
		static constexpr T increment = c;

		/**
		 * @brief The modulus value
		 *
		 */
		static constexpr T modulus = m;

		/**
		 * @brief The default seed value
		 *
		 */
		static constexpr T default_seed = 1;

		/**
		 * @brief Construct a new linear congruential engine object
		 *
		 */
		linear_congruential_engine() : linear_congruential_engine(default_seed) {}

		/**
		 * @brief Construct a new linear congruential engine object
		 *
		 * @param s The seed for the random number generator
		 */
		explicit linear_congruential_engine(T s) {
			seed(s);
		}

		// template <typename Sseq>
		// explicit linear_congruential_engine(Sseq &q) {}
		// TODO implement this - requires std::seed_seq

		/**
		 * @brief Seed the random number generator
		 *
		 * @param seed The seed for the random number generator
		 */
		void seed(T seed) {
			if (c % m == 0 && seed % m == 0) {
				_state = 1;
			} else {
				_state = seed % m;
			}
		}

		// template <typename Sseq>
		// void seed(Sseq &q) {}
		// TODO implement this - requires std::seed_seq

		/**
		 * @brief Generate a random number
		 *
		 * @return The random number
		 */
		T operator()() {
			_state = (_state * a + c) % m;
			return _state;
		}

		/**
		 * @brief Discard the next z numbers from the random number generator
		 *
		 * @param z The number of random numbers to discard
		 */
		void discard(unsigned long long z) {
			for (z; z > 0; z--) {
				(*this)();
			}
		}

		/**
		 * @brief Returns the minimum value that can be generated
		 *
		 * @return The minimum value that can be generated
		 */
		static constexpr T min() {
			if constexpr (c == 0) {
				return 1;
			} else {
				return 0;
			}
		}

		/**
		 * @brief Returns the maximum value that can be generated
		 *
		 * @return The maximum value that can be generated
		 */
		static constexpr T max() {
			return m - 1;
		}

		/**
		 * @brief Check for equality between two linear congruential engines
		 *
		 * @param lhs The first linear congruential engine
		 * @param rhs The second linear congruential engine
		 * @return true If the two engines are equal
		 */
		friend bool operator==(const linear_congruential_engine &lhs, const linear_congruential_engine &rhs) {
			return lhs._state == rhs._state;
		}

	  private:
		T _state;
	};

	/**
	 * @brief A linear congruential engine using the minstd_rand0 parameters
	 *
	 */
	using minstd_rand0 = linear_congruential_engine<uint_fast32_t, 16807UL, 0UL, 2147483647UL>;

	/**
	 * @brief A linear congruential engine using the minstd_rand parameters
	 *
	 */
	using minstd_rand = linear_congruential_engine<uint_fast32_t, 48271UL, 0UL, 2147483647UL>;
}