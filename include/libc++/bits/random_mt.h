/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-02-04
 * @brief Random number generator using a Mersenne Twister engine
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define __need_size_t
#include <stddef.h>

#include <limits>
#include <stdint.h>

namespace std {
	namespace internal {
		template <typename T, size_t sz, bool = (sz < static_cast<size_t>(std::numeric_limits<T>::digits))>
		struct _shift {
			static constexpr T __value = 0;
		};
		template <typename T, size_t sz>
		struct _shift<T, sz, true> {
			static constexpr T __value = T(1) << sz;
		};
		template <typename T, size_t sz>
		constexpr inline auto _shift_v = _shift<T, sz>::__value;
	}

	/**
	 * @brief Random number generator using a Mersenne Twister engine
	 *
	 * @tparam T The type of the random number
	 * @tparam w The word size
	 * @tparam n The state size
	 * @tparam m The shift size
	 * @tparam r The number of bits in the lower bitmask
	 * @tparam a The xor mask
	 * @tparam u The first tempering shift
	 * @tparam d The first tempering mask
	 * @tparam s The second tempering shift
	 * @tparam b The second tempering mask
	 * @tparam t The third tempering shift
	 * @tparam c The third tempering mask
	 * @tparam l The fourth tempering shift
	 * @tparam f The initialization multiplier
	 */
	template <typename T, size_t w, size_t n, size_t m, size_t r, T a, size_t u, T d, size_t s, T b, size_t t, T c, size_t l, T f>
	class mersenne_twister_engine {
		static_assert(std::is_integral_v<T>, "T must be an integral type");
		static_assert(std::is_unsigned_v<T>, "T must be an unsigned type");
		static_assert(0 < m && m < n, "Invalid parameters for mersenne_twister_engine");
		static_assert(2 < w, "Invalid parameters for mersenne_twister_engine");
		static_assert(r <= w, "Invalid parameters for mersenne_twister_engine");
		static_assert(u <= w, "Invalid parameters for mersenne_twister_engine");
		static_assert(s <= w, "Invalid parameters for mersenne_twister_engine");
		static_assert(t <= w, "Invalid parameters for mersenne_twister_engine");
		static_assert(l <= w, "Invalid parameters for mersenne_twister_engine");
		static_assert(w <= std::numeric_limits<T>::digits, "Invalid parameters for mersenne_twister_engine");
		static_assert(a <= internal::_shift_v<T, w> - 1, "Invalid parameters for mersenne_twister_engine");
		static_assert(b <= internal::_shift_v<T, w> - 1, "Invalid parameters for mersenne_twister_engine");
		static_assert(c <= internal::_shift_v<T, w> - 1, "Invalid parameters for mersenne_twister_engine");
		static_assert(d <= internal::_shift_v<T, w> - 1, "Invalid parameters for mersenne_twister_engine");
		static_assert(f <= internal::_shift_v<T, w> - 1, "Invalid parameters for mersenne_twister_engine");

	  private:
		T _state[n];
		size_t _index;

		/**
		 * @brief Twist the internal state
		 *
		 */
		void __twist(void) {
			const T upper_mask = (~T()) << r;
			const T lower_mask = ~upper_mask;

			for (size_t k = 0; k < n - m; k++) {
				T x = (_state[k] & upper_mask) + (_state[k + 1] & lower_mask);
				_state[k] = _state[k + m] ^ (x >> 1);
				if (x & 1) {
					_state[k] ^= a;
				}
			}

			for (size_t k = n - m; k < n - 1; k++) {
				T x = (_state[k] & upper_mask) + (_state[k + 1] & lower_mask);
				_state[k] = _state[k + (m - n)] ^ (x >> 1);
				if (x & 1) {
					_state[k] ^= a;
				}
			}

			T x = (_state[n - 1] & upper_mask) + (_state[0] & lower_mask);
			_state[n - 1] = _state[m - 1] ^ (x >> 1);
			if (x & 1) {
				_state[n - 1] ^= a;
			}

			_index = 0;
		}

	  public:
		/**
		 * @brief The word size
		 *
		 */
		static constexpr size_t word_size = w;

		/**
		 * @brief The state size
		 *
		 */
		static constexpr size_t state_size = n;

		/**
		 * @brief The shift size
		 *
		 */
		static constexpr size_t shift_size = m;

		/**
		 * @brief The number of bits in the lower bitmask
		 *
		 */
		static constexpr size_t mask_bits = r;

		/**
		 * @brief The xor mask
		 *
		 */
		static constexpr T xor_mask = a;

		/**
		 * @brief The first tempering shift
		 *
		 */
		static constexpr size_t tempering_u = u;

		/**
		 * @brief The first tempering mask
		 *
		 */
		static constexpr T tempering_d = d;

		/**
		 * @brief The second tempering shift
		 *
		 */
		static constexpr size_t tempering_s = s;

		/**
		 * @brief The second tempering mask
		 *
		 */
		static constexpr T tempering_b = b;

		/**
		 * @brief The third tempering shift
		 *
		 */
		static constexpr size_t tempering_t = t;

		/**
		 * @brief The third tempering mask
		 *
		 */
		static constexpr T tempering_c = c;

		/**
		 * @brief The fourth tempering shift
		 *
		 */
		static constexpr size_t tempering_l = l;

		/**
		 * @brief The initialization multiplier
		 *
		 */
		static constexpr T initialization_multiplier = f;

		/**
		 * @brief The default seed value
		 *
		 */
		static constexpr T default_seed = 5489;

		/**
		 * @brief Construct a new mersenne twister engine object
		 *
		 */
		mersenne_twister_engine() : mersenne_twister_engine(default_seed) {}

		/**
		 * @brief Construct a new mersenne twister engine object
		 *
		 * @param value The seed for the random number generator
		 */
		explicit mersenne_twister_engine(T value) {
			seed(value);
		}

		// template <typename Sseq>
		// explicit mersenne_twister_engine(Sseq &seq) {}
		// TODO implement this - requires std::seed_seq

		/**
		 * @brief Seed the random number generator
		 *
		 * @param value The seed for the random number generator
		 */
		void seed(T value = default_seed) {
			if constexpr (w < std::numeric_limits<T>::digits) {
				_state[0] = value & ((T(1) << w) - 1);
			} else {
				_state[0] = value;
			}

			for (size_t i = 1; i < n; i++) {
				T prev = _state[i - 1];
				prev ^= (prev >> (w - 2));
				prev *= f;
				prev += i % n;

				if constexpr (w < std::numeric_limits<T>::digits) {
					_state[i] = prev & ((T(1) << w) - 1);
				} else {
					_state[i] = prev;
				}
			}

			_index = n;
		}

		// template <typename Sseq>
		// void seed(Sseq &seq) {}
		// TODO implement this - requires std::seed_seq

		/**
		 * @brief Generate a random number
		 *
		 * @return The random number
		 */
		T operator()() {
			if (_index >= n) {
				__twist();
			}

			T result = _state[_index];
			result ^= (result >> u) & d;
			result ^= (result << s) & b;
			result ^= (result << t) & c;
			result ^= (result >> l);

			_index++;
			return result;
		}

		/**
		 * @brief Discard the next z elements of the random number generator
		 *
		 * @param z The number of elements to discard
		 */
		void discard(unsigned long long z) {
			while (z > n - _index) {
				z -= n - _index;
				__twist();
			}
			_index += z;
		}

		/**
		 * @brief Returns the minimum value that can be generated
		 *
		 * @return The minimum value that can be generated
		 */
		static constexpr T min() {
			return 0;
		}

		/**
		 * @brief Returns the maximum value that can be generated
		 *
		 * @return The maximum value that can be generated
		 */
		static constexpr T max() {
			return (T(1) << w) - 1;
		}

		/**
		 * @brief Checks for equality between two mersenne twister engines
		 *
		 * @param lhs The first mersenne twister engine
		 * @param rhs The second mersenne twister engine
		 * @return true If the two engines are equal
		 */
		friend bool operator==(const mersenne_twister_engine &lhs, const mersenne_twister_engine &rhs) {
			if (lhs._index != rhs._index) {
				return false;
			}
			for (size_t i = 0; i < n; i++) {
				if (lhs._state[i] != rhs._state[i]) {
					return false;
				}
			}
			// TODO use std::equal
			return true;
		}
	};

	/**
	 * @brief A mersenne twister engine using the mt19937 parameters
	 *
	 */
	using mt19937 = mersenne_twister_engine<
		uint_fast32_t,
		32, 624, 397, 31,
		0x9908b0dfUL, 11,
		0xffffffffUL, 7,
		0x9d2c5680UL, 15,
		0xefc60000UL, 18, 1812433253UL>;

	/**
	 * @brief A mersenne twister engine using the mt19937_64 parameters
	 *
	 */
	using mt19937_64 = mersenne_twister_engine<
		uint_fast64_t,
		64, 312, 156, 31,
		0xb5026f5aa96619e9ULL, 29,
		0x5555555555555555ULL, 17,
		0x71d67fffeda60000ULL, 37,
		0xfff7eee000000000ULL, 43,
		6364136223846793005ULL>;
}