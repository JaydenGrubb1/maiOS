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
		enum class __arg_type {
			NONE,
			CHAR,
			BOOL,
			INT,
			UINT,
			LONG,
			ULONG,
			FLOAT,
			DOUBLE,
			POINTER,
			CSTRING,
			STRING,
			CUSTOM
		} _type = __arg_type::NONE;

		union __arg_value {
			monostate _none;
			Char _char;
			bool _bool;
			signed int _int;
			unsigned int _uint;
			signed long long _long;
			unsigned long long _ulong;
			float _float;
			double _double;
			const void *_pointer;
			const Char *_cstring;
			basic_string_view<Char> _string;
			handle _handle;
		} _value = {};

	  public:
		template <typename T>
		consteval basic_format_arg(type_identity<T>) {
			if constexpr (std::is_same_v<T, Char>) {
				_type = __arg_type::CHAR;
				_value._char = {};
			} else if constexpr (std::is_same_v<T, bool>) {
				_type = __arg_type::BOOL;
				_value._bool = {};
			} else if constexpr (std::is_same_v<T, signed int>) {
				_type = __arg_type::INT;
				_value._int = {};
			} else if constexpr (std::is_same_v<T, unsigned int>) {
				_type = __arg_type::UINT;
				_value._uint = {};
			} else if constexpr (std::is_same_v<T, signed long long>) {
				_type = __arg_type::LONG;
				_value._long = {};
			} else if constexpr (std::is_same_v<T, unsigned long long>) {
				_type = __arg_type::ULONG;
				_value._ulong = {};
			} else if constexpr (std::is_same_v<T, float>) {
				_type = __arg_type::FLOAT;
				_value._float = {};
			} else if constexpr (std::is_same_v<T, double>) {
				_type = __arg_type::DOUBLE;
				_value._double = {};
			} else if constexpr (std::is_pointer_v<T>) {
				_type = __arg_type::POINTER;
				_value._pointer = {};
			} else if constexpr (std::is_same_v<T, const Char *>) {
				_type = __arg_type::CSTRING;
				_value._cstring = {};
			} else if constexpr (std::is_convertible_v<T, basic_string_view<Char>>) {
				_type = __arg_type::STRING;
				_value._string = {};
			} else {
				_type = __arg_type::CUSTOM;
				_value._handle = handle{type_identity<T>{}};
			}
		}

		constexpr explicit basic_format_arg(void)
			: _type(__arg_type::NONE), _value({._none = {}}) {}

		constexpr explicit basic_format_arg(Char value)
			: _type(__arg_type::CHAR), _value({._char = value}) {}

		constexpr explicit basic_format_arg(bool value)
			: _type(__arg_type::BOOL), _value({._bool = value}) {}

		constexpr explicit basic_format_arg(signed int value)
			: _type(__arg_type::INT), _value({._int = value}) {}

		constexpr explicit basic_format_arg(unsigned int value)
			: _type(__arg_type::UINT), _value({._uint = value}) {}

		constexpr explicit basic_format_arg(signed long long value)
			: _type(__arg_type::LONG), _value({._long = value}) {}

		constexpr explicit basic_format_arg(unsigned long long value)
			: _type(__arg_type::ULONG), _value({._ulong = value}) {}

		constexpr explicit basic_format_arg(float value)
			: _type(__arg_type::FLOAT), _value({._float = value}) {}

		constexpr explicit basic_format_arg(double value)
			: _type(__arg_type::DOUBLE), _value({._double = value}) {}

		constexpr explicit basic_format_arg(nullptr_t)
			: _type(__arg_type::POINTER), _value({._pointer = 0}) {}

		constexpr explicit basic_format_arg(const void *value)
			: _type(__arg_type::POINTER), _value({._pointer = value}) {}

		constexpr explicit basic_format_arg(const Char *value)
			: _type(__arg_type::CSTRING), _value({._cstring = value}) {}

		constexpr explicit basic_format_arg(basic_string_view<Char> value)
			: _type(__arg_type::STRING), _value({._string = value}) {}

		template <typename T>
		constexpr explicit basic_format_arg(const T &value)
			requires(!(std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_pointer_v<T> || std::is_convertible_v<T, basic_string_view<Char>>))
			: _type(__arg_type::CUSTOM), _value({._handle = handle{value}}) {}

		[[nodiscard]] explicit inline operator bool() const {
			return _type != __arg_type::NONE;
		}

		template <typename Visitor>
		constexpr decltype(auto) visit(__arg_type type, Visitor &&visitor) const {
			switch (type) {
				case __arg_type::NONE:
					return std::forward<Visitor>(visitor)(_value._none);
				case __arg_type::CHAR:
					return std::forward<Visitor>(visitor)(_value._char);
				case __arg_type::BOOL:
					return std::forward<Visitor>(visitor)(_value._bool);
				case __arg_type::INT:
					return std::forward<Visitor>(visitor)(_value._int);
				case __arg_type::UINT:
					return std::forward<Visitor>(visitor)(_value._uint);
				case __arg_type::LONG:
					return std::forward<Visitor>(visitor)(_value._long);
				case __arg_type::ULONG:
					return std::forward<Visitor>(visitor)(_value._ulong);
				case __arg_type::FLOAT:
					return std::forward<Visitor>(visitor)(_value._float);
				case __arg_type::DOUBLE:
					return std::forward<Visitor>(visitor)(_value._double);
				case __arg_type::POINTER:
					return std::forward<Visitor>(visitor)(_value._pointer);
				case __arg_type::CSTRING:
					return std::forward<Visitor>(visitor)(_value._cstring);
				case __arg_type::STRING:
					return std::forward<Visitor>(visitor)(_value._string);
				case __arg_type::CUSTOM:
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