/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-07
 * @brief Class template for representing a duration of time
 * @link https://en.cppreference.com/w/cpp/chrono/duration @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <limits>
#include <ratio>
#include <type_traits>

namespace std {
	namespace chrono {
		template <typename T, typename P = std::ratio<1>>
		class duration;

		template <typename R, typename T, typename P>
		[[nodiscard]] constexpr R duration_cast(const duration<T, P> &other) {
			if constexpr (std::is_same_v<R, duration<T, P>>) {
				return other;
			} else {
				using U = typename R::rep;
				using Q = typename R::period;
				using Com = typename std::common_type<U, T, intmax_t>::type;
				using Div = std::ratio_divide<P, Q>;

				if constexpr (Div::num == 1) {
					if constexpr (Div::den == 1) {
						return R(static_cast<U>(other.count()));
					} else {
						return R(static_cast<U>(other.count() / Div::den));
					}
				} else {
					if constexpr (Div::den == 1) {
						return R(static_cast<U>(other.count() * Div::num));
					} else {
						return R(static_cast<U>(static_cast<Com>(other.count()) * Div::num / Div::den));
					}
				}
			}
		}

		template <typename T>
		struct duration_values {
			[[nodiscard]] static constexpr T zero() {
				return T(0);
			}

			[[nodiscard]] static constexpr T min() {
				return std::numeric_limits<T>::lowest();
			}

			[[nodiscard]] static constexpr T max() {
				return std::numeric_limits<T>::max();
			}
		};

		template <typename T, typename P>
		class duration {
		  public:
			using rep = T;
			using period = P;

		  private:
			T _value;

		  public:
			constexpr duration() = default;

			constexpr duration(const duration &) = default;

			template <typename U>
			constexpr explicit duration(const U &other) : _value(static_cast<T>(other)) {}

			template <typename U, typename Q>
			constexpr duration(const duration<U, Q> &other) : _value(duration_cast<duration>(other).count()) {}

			~duration() = default;
			duration &operator=(const duration &) = default;

			[[nodiscard]] constexpr T count() const {
				return _value;
			}

			[[nodiscard]] static constexpr duration zero() {
				return duration(duration_values<T>::zero());
			}

			[[nodiscard]] static constexpr duration min() {
				return duration(duration_values<T>::min());
			}

			[[nodiscard]] static constexpr duration max() {
				return duration(duration_values<T>::max());
			}

			// TODO arithmetic operators
		};

		// TODO arithmetic operators
		// TODO comparison operators
		// TODO floor
		// TODO ceil
		// TODO round
		// TODO abs
		// TODO hash

		using nanoseconds = duration<int64_t, std::nano>;
		using microseconds = duration<int64_t, std::micro>;
		using milliseconds = duration<int64_t, std::milli>;
		using seconds = duration<int64_t>;
		using minutes = duration<int64_t, std::ratio<60>>;
		using hours = duration<int64_t, std::ratio<3600>>;
		using days = duration<int64_t, std::ratio<86400>>;
		using weeks = duration<int64_t, std::ratio<604800>>;
		using months = duration<int64_t, std::ratio<2629746>>;
		using years = duration<int64_t, std::ratio<31556952>>;
	}

	inline namespace literals {
		inline namespace chrono_literals {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"
			inline constexpr chrono::nanoseconds operator""ns(unsigned long long val) {
				return chrono::nanoseconds(val);
			}

			inline constexpr chrono::microseconds operator""us(unsigned long long val) {
				return chrono::microseconds(val);
			}

			inline constexpr chrono::milliseconds operator""ms(unsigned long long val) {
				return chrono::milliseconds(val);
			}

			inline constexpr chrono::seconds operator""s(unsigned long long val) {
				return chrono::seconds(val);
			}

			inline constexpr chrono::minutes operator""min(unsigned long long val) {
				return chrono::minutes(val);
			}

			inline constexpr chrono::hours operator""h(unsigned long long val) {
				return chrono::hours(val);
			}
#pragma GCC diagnostic pop
		}
	}

	namespace chrono {
		using namespace literals::chrono_literals;
	}
}