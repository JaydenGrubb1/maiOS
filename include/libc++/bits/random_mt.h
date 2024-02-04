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

#include <stdint.h>

namespace std {
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
		// TODO numeric limit
		// static_assert(a <= (1 << w) - 1, "Invalid parameters for mersenne_twister_engine");
		// static_assert(b <= (1 << w) - 1, "Invalid parameters for mersenne_twister_engine");
		// static_assert(c <= (1 << w) - 1, "Invalid parameters for mersenne_twister_engine");
		// static_assert(d <= (1 << w) - 1, "Invalid parameters for mersenne_twister_engine");
		// static_assert(f <= (1 << w) - 1, "Invalid parameters for mersenne_twister_engine");

	  private:
		T _state[n];
		size_t _index;

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
		static constexpr size_t word_size = w;
		static constexpr size_t state_size = n;
		static constexpr size_t shift_size = m;
		static constexpr size_t mask_bits = r;
		static constexpr T xor_mask = a;
		static constexpr size_t tempering_u = u;
		static constexpr T tempering_d = d;
		static constexpr size_t tempering_s = s;
		static constexpr T tempering_b = b;
		static constexpr size_t tempering_t = t;
		static constexpr T tempering_c = c;
		static constexpr size_t tempering_l = l;
		static constexpr T initialization_multiplier = f;
		static constexpr T default_seed = 5489;

		mersenne_twister_engine() : mersenne_twister_engine(default_seed) {}

		explicit mersenne_twister_engine(T value) {
			seed(value);
		}

		// template <typename Sseq>
		// explicit mersenne_twister_engine(Sseq &seq) {}
		// TODO implement this - requires std::seed_seq

		void seed(T value = default_seed) {
			// TODO handle sizof(T) < w
			// _state[0] = value % (1 << w);
			_state[0] = value;

			for (size_t i = 1; i < n; i++) {
				T prev = _state[i - 1];
				prev ^= (prev >> (w - 2));
				prev *= f;
				prev += i % n;
				// _state[i] = prev % (1 << w);
				_state[i] = prev;
			}

			_index = n;
		}

		// template <typename Sseq>
		// void seed(Sseq &seq) {}
		// TODO implement this - requires std::seed_seq

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

		void discard(unsigned long long z) {
			while (z > n - _index) {
				z -= n - _index;
				__twist();
			}
			_index += z;
		}

		static constexpr T min() {
			return 0;
		}

		static constexpr T max() {
			return (1 << w) - 1;
		}

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

	using mt19937 = mersenne_twister_engine<
		uint_fast32_t,
		32, 624, 397, 31,
		0x9908b0dfUL, 11,
		0xffffffffUL, 7,
		0x9d2c5680UL, 15,
		0xefc60000UL, 18, 1812433253UL>;

	using mt19937_64 = mersenne_twister_engine<
		uint_fast64_t,
		64, 312, 156, 31,
		0xb5026f5aa96619e9ULL, 29,
		0x5555555555555555ULL, 17,
		0x71d67fffeda60000ULL, 37,
		0xfff7eee000000000ULL, 43,
		6364136223846793005ULL>;
}