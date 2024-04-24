/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Provides access to a single type-erased argument for formatting functions
 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_arg @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/fmt/format_fwd.h>
#include <utility>
#include <variant>

namespace std {
	template <typename Context>
	class basic_format_arg {
	  private:
		using Char = typename Context::char_type;

	  public:
		class handle {
		  private:
			const void *_ptr;
			void (*_func)(basic_format_parse_context<Char> &, Context &, const void *);

			template <typename T>
			constexpr static void __format(basic_format_parse_context<Char> &parse_ctx, Context &fmt_ctx, const void *ptr) {
				using Type = std::remove_const_t<T>;
				using Formatter = typename Context::template formatter_type<Type>;

				Formatter fmt;
				parse_ctx.advance_to(fmt.parse(parse_ctx));

				if (!std::is_constant_evaluated()) {
					Type &val = *const_cast<Type *>(static_cast<const T *>(ptr));
					fmt_ctx.advance_to(fmt.format(val, fmt_ctx));
				}
			}

			template <typename T>
			consteval explicit handle(type_identity<T>)
				: _ptr(nullptr), _func(__format<T>) {}

			template <typename T>
			constexpr explicit handle(const T &value)
				: _ptr(std::addressof(value)), _func(__format<T>) {}

			friend class basic_format_arg<Context>;

		  public:
			constexpr void format(basic_format_parse_context<Char> &parse_ctx, Context &fmt_ctx) const {
				_func(parse_ctx, fmt_ctx, _ptr);
			}
		};

	  private:
		enum struct __arg_type {
			none,
			character,
			boolean,
			signed_integer,
			unsigned_integer,
			signed_long,
			unsigned_long,
			floating_point,
			double_precision,
			pointer,
			cstring,
			string,
			custom
		} _type = __arg_type::none;

		union __arg_value {
			monostate _none;
			Char _character;
			bool _boolean;
			signed int _signed_integer;
			unsigned int _unsigned_integer;
			signed long long _signed_long;
			unsigned long long _unsigned_long;
			float _float;
			double _double;
			void *_pointer;
			const Char *_cstring;
			basic_string_view<Char> _string;
			handle _handle;
		} _value = {};

	  public:
		template <typename T>
		consteval basic_format_arg(type_identity<T>) {
			if constexpr (std::is_same_v<T, Char>) {
				_type = __arg_type::character;
				_value._character = {};
			} else if constexpr (std::is_same_v<T, bool>) {
				_type = __arg_type::boolean;
				_value._boolean = {};
			} else if constexpr (std::is_same_v<T, signed int>) {
				_type = __arg_type::signed_integer;
				_value._signed_integer = {};
			} else if constexpr (std::is_same_v<T, unsigned int>) {
				_type = __arg_type::unsigned_integer;
				_value._unsigned_integer = {};
			} else if constexpr (std::is_same_v<T, signed long long>) {
				_type = __arg_type::signed_long;
				_value._signed_long = {};
			} else if constexpr (std::is_same_v<T, unsigned long long>) {
				_type = __arg_type::unsigned_long;
				_value._unsigned_long = {};
			} else if constexpr (std::is_same_v<T, float>) {
				_type = __arg_type::floating_point;
				_value._float = {};
			} else if constexpr (std::is_same_v<T, double>) {
				_type = __arg_type::double_precision;
				_value._double = {};
			} else if constexpr (std::is_pointer_v<T>) {
				_type = __arg_type::pointer;
				_value._pointer = {};
			} else if constexpr (std::is_same_v<T, const Char *>) {
				_type = __arg_type::cstring;
				_value._cstring = {};
			} else if constexpr (std::is_convertible_v<T, basic_string_view<Char>>) {
				_type = __arg_type::string;
				_value._string = {};
			} else {
				_type = __arg_type::custom;
				_value._handle = handle{type_identity<T>{}};
			}
		}

		constexpr explicit basic_format_arg(void)
			: _type(__arg_type::none), _value({._none = {}}) {}

		constexpr explicit basic_format_arg(Char value)
			: _type(__arg_type::character), _value({._character = value}) {}

		constexpr explicit basic_format_arg(bool value)
			: _type(__arg_type::boolean), _value({._boolean = value}) {}

		constexpr explicit basic_format_arg(signed int value)
			: _type(__arg_type::signed_integer), _value({._signed_integer = value}) {}

		constexpr explicit basic_format_arg(unsigned int value)
			: _type(__arg_type::unsigned_integer), _value({._unsigned_integer = value}) {}

		constexpr explicit basic_format_arg(signed long long value)
			: _type(__arg_type::signed_long), _value({._signed_long = value}) {}

		constexpr explicit basic_format_arg(unsigned long long value)
			: _type(__arg_type::unsigned_long), _value({._unsigned_long = value}) {}

		constexpr explicit basic_format_arg(float value)
			: _type(__arg_type::floating_point), _value({._float = value}) {}

		constexpr explicit basic_format_arg(double value)
			: _type(__arg_type::double_precision), _value({._double = value}) {}

		constexpr explicit basic_format_arg(nullptr_t)
			: _type(__arg_type::pointer), _value({._pointer = 0}) {}

		constexpr explicit basic_format_arg(void *value)
			: _type(__arg_type::pointer), _value({._pointer = value}) {}

		constexpr explicit basic_format_arg(const Char *value)
			: _type(__arg_type::cstring), _value({._cstring = value}) {}

		constexpr explicit basic_format_arg(basic_string_view<Char> value)
			: _type(__arg_type::string), _value({._string = value}) {}

		template <typename T>
		constexpr explicit basic_format_arg(const T &value)
			requires(!(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_pointer_v<T> || std::is_convertible_v<T, basic_string_view<Char>>))
			: _type(__arg_type::custom), _value({._handle = handle{value}}) {}

		[[nodiscard]] explicit inline operator bool() const {
			return _type != __arg_type::none;
		}

		template <typename Visitor>
		constexpr decltype(auto) visit(__arg_type type, Visitor &&visitor) const {
			switch (type) {
				case __arg_type::none:
					return std::forward<Visitor>(visitor)(_value._none);
				case __arg_type::character:
					return std::forward<Visitor>(visitor)(_value._character);
				case __arg_type::boolean:
					return std::forward<Visitor>(visitor)(_value._boolean);
				case __arg_type::signed_integer:
					return std::forward<Visitor>(visitor)(_value._signed_integer);
				case __arg_type::unsigned_integer:
					return std::forward<Visitor>(visitor)(_value._unsigned_integer);
				case __arg_type::signed_long:
					return std::forward<Visitor>(visitor)(_value._signed_long);
				case __arg_type::unsigned_long:
					return std::forward<Visitor>(visitor)(_value._unsigned_long);
				case __arg_type::floating_point:
					return std::forward<Visitor>(visitor)(_value._float);
				case __arg_type::double_precision:
					return std::forward<Visitor>(visitor)(_value._double);
				case __arg_type::pointer:
					return std::forward<Visitor>(visitor)(_value._pointer);
				case __arg_type::cstring:
					return std::forward<Visitor>(visitor)(_value._cstring);
				case __arg_type::string:
					return std::forward<Visitor>(visitor)(_value._string);
				case __arg_type::custom:
					return std::forward<Visitor>(visitor)(_value._handle);
				default:
					std::unreachable();
			}
		}

		template <typename Visitor, typename Ctx>
		constexpr friend decltype(auto) visit_format_arg(Visitor &&, basic_format_arg<Ctx>);
	};

	template <typename Visitor, typename Context>
	constexpr decltype(auto) visit_format_arg(Visitor &&visitor, basic_format_arg<Context> arg) {
		return arg.visit(arg._type, std::forward<Visitor>(visitor));
	}
}