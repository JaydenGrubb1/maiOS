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

#include <bits/fmt/forward.h>
#include <utility>
#include <variant>

namespace std {
	template <typename Context>
	class basic_format_arg {
	  public:
		class handle {
			// TODO implement this
		};

	  private:
		using Char = typename Context::char_type;

		enum struct __arg_type {
			none,
			character,
			boolean,
			signed_integer,
			unsigned_integer,
			// TODO add more types
			// TODO add handle
		} _type = __arg_type::none;

		union __arg_value {
			monostate _none;
			Char _character;
			bool _boolean;
			signed int _signed_integer;
			unsigned int _unsigned_integer;
			// TODO add more types
			// TODO add handle
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
			}
			// TODO add more types
			// TODO add handle
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
		// TODO add more types
		// TODO add handle

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
					// TODO add more types
					// TODO add handle
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