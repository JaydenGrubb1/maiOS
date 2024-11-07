/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-22
 * @brief Defines formatting rules for a type
 * @link https://en.cppreference.com/w/cpp/utility/format/formatter @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/fmt/format_fwd.h>
#include <iterator> // HACK should probably just use an iterator that doesn't require this (not back_insert_iterator)

namespace std {
	namespace __detail {
		template <typename Char, typename T>
		consteval basic_string_view<Char> __type_name(void) {
			if constexpr (std::is_same_v<Char, wchar_t>) {
				// FIXME support wide characters
				return L"unknown";
			}

#if defined(__GNUC__) && !defined(__clang__)
			auto name = basic_string_view<Char>(__PRETTY_FUNCTION__);
			auto start = name.find(basic_string_view<Char>("T = "));
			auto end = name.find(basic_string_view<Char>("]"), start);
			return name.substr(start + 4, end - start - 4);
#else
			return "unknown";
#endif
		}
	}

	/**
	 * @brief Extension namespace for parsing format strings
	 *
	 */
	namespace fmt {
		enum class alignment {
			UNKNOWN = 0,
			LEFT,
			RIGHT,
			CENTER
		};

		template <typename Char>
		constexpr alignment parse_alignment(Char ch) {
			switch (ch) {
				case Char('<'):
					return alignment::LEFT;
				case Char('>'):
					return alignment::RIGHT;
				case Char('^'):
					return alignment::CENTER;
				default:
					return alignment::UNKNOWN;
			}
		}

		template <typename Char, typename Iter>
		constexpr Iter parse_fill(Iter begin, Iter end, Char &fill, alignment &align) {
			if (begin == end) {
				return begin;
			}

			if (std::distance(begin, end) >= 2) {
				align = parse_alignment(begin[1]);
				if (align != alignment::UNKNOWN) {
					fill = begin[0];
					std::advance(begin, 2);
					return begin;
				}
			}

			align = parse_alignment(begin[0]);
			if (align != alignment::UNKNOWN) {
				fill = Char(' ');
				std::advance(begin, 1);
				return begin;
			}

			return begin;
		}

		template <typename Char, typename Iter>
		constexpr Iter parse_width(Iter begin, Iter end, int &width) {
			if (begin == end) {
				return begin;
			}

			if (*begin >= Char('0') && *begin <= Char('9')) {
				width = *begin - '0';
				std::advance(begin, 1);
				return begin;
			}
			// TODO parse more digits
			// TODO parse width from argument

			return begin;
		}

		template <typename Char, typename Iter>
		constexpr Iter parse_precision(Iter begin, Iter end, int &precision) {
			if (begin == end) {
				return begin;
			}

			if (*begin == Char('.')) {
				++begin;
				if (*begin >= Char('0') && *begin <= Char('9')) {
					precision = *begin - '0';
					std::advance(begin, 1);
					return begin;
				} else {
					std::unreachable();
				}
			}
			// TODO parse more digits
			// TODO parse precision from argument

			return begin;
		}
	}

#ifdef DNDEBUG
	template <typename T, typename Char>
	struct formatter;
#else
	template <typename T, typename Char>
	struct formatter {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T &value, basic_format_context<Iter, Char> &ctx) {
			auto name = __detail::__type_name<Char, T>();
			auto addr = std::addressof(value);
			return format_to(ctx.out(), "<{} @ {}>", name, addr);
		}
	};
#endif

	template <typename Char>
	struct formatter<Char, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const Char value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('c');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<bool, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const bool value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('b');
			return ctx.out();
		}
	};

	template <typename T, typename Char>
		requires std::is_integral_v<T>
	struct formatter<T, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('i');
			return ctx.out();
		}
	};

	template <typename T, typename Char>
		requires std::is_floating_point_v<T>
	struct formatter<T, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('f');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<basic_string_view<Char>, Char> {
		fmt::alignment _align = fmt::alignment::UNKNOWN;
		Char _fill = Char(' ');
		int _width = -1;
		int _precision = -1;

		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			ctx.advance_to(fmt::parse_fill(ctx.begin(), ctx.end(), _fill, _align));
			ctx.advance_to(fmt::parse_width<Char>(ctx.begin(), ctx.end(), _width));
			ctx.advance_to(fmt::parse_precision<Char>(ctx.begin(), ctx.end(), _precision));

			if (*ctx.begin() == Char('s')) {
				ctx.advance_to(ctx.begin() + 1);
			}

			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const basic_string_view<Char> value, basic_format_context<Iter, Char> &ctx) {
			int len = std::min(static_cast<size_t>(_precision), value.size());
			int leading = 0;
			int trailing = 0;

			_width -= len;

			switch (_align) {
				default:
					[[fallthrough]];
				case fmt::alignment::LEFT:
					trailing = _width;
					break;
				case fmt::alignment::RIGHT:
					leading = _width;
					break;
				case fmt::alignment::CENTER:
					leading = _width / 2;
					trailing = _width - leading;
					break;
			}

			for (int i = 0; i < leading; i++) {
				*ctx.out()++ = _fill;
			}
			for (int i = 0; i < len; i++) {
				*ctx.out()++ = value[i];
			}
			for (int i = 0; i < trailing; i++) {
				*ctx.out()++ = _fill;
			}

			// TODO handle escaped characters
			// TODO don't break multibyte characters

			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<const Char *, Char> : formatter<basic_string_view<Char>, Char> {};

	template <typename Char>
	struct formatter<const void *, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const void *value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('p');
			return ctx.out();
		}
	};

}
