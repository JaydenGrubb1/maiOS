/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-05
 * @brief Adaptor class template providing FIFO access to a data structure
 * @link https://en.cppreference.com/w/cpp/container/queue @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <bits/iterator_traits.h>
#include <list> // TODO replace with <deque>
#include <utility>

namespace std {
	template <typename T, typename S = list<T>> // TODO replace with deque<T>
	class queue {
	  public:
		using container_type = S;
		using value_type = typename S::value_type;
		using size_type = typename S::size_type;
		using reference = typename S::reference;
		using const_reference = typename S::const_reference;

	  private:
		S _sequence;

	  public:
#pragma region Constructors
		constexpr queue(void)
			requires(std::is_default_constructible_v<S>)
			: _sequence() {}

		constexpr explicit queue(const S &other) : _sequence(other) {}

		constexpr explicit queue(S &&other) : _sequence(std::move(other)) {}

		constexpr queue(const queue &other) : _sequence(other._sequence) {}

		constexpr queue(queue &&other) : _sequence(std::move(other._sequence)) {}

// TODO iterator constructor

// TODO allocator aware constructors (requires std::uses_allocator)
#pragma endregion

		[[nodiscard]] constexpr bool empty(void) const {
			return _sequence.empty();
		}

		[[nodiscard]] constexpr size_type size(void) const {
			return _sequence.size();
		}

		[[nodiscard]] T &front(void) {
			return _sequence.front();
		}

		[[nodiscard]] const T &front(void) const {
			return _sequence.front();
		}

		[[nodiscard]] T &back(void) {
			return _sequence.back();
		}

		[[nodiscard]] const T &back(void) const {
			return _sequence.back();
		}

		constexpr void push(const T &value) {
			_sequence.push_back(value);
		}

		constexpr void push(T &&value) {
			_sequence.push_back(std::move(value));
		}

		template <typename... Args>
		constexpr decltype(auto) emplace(Args &&...args) {
			return _sequence.emplace_back(std::forward<Args>(args)...);
		}

		constexpr void pop(void) {
			_sequence.pop_front();
		}

		constexpr void swap(queue &other) {
			_sequence.swap(other._sequence);
		}

		template <typename U, typename D>
		constexpr friend bool operator==(const queue<U, D> &lhs, const queue<U, D> &rhs);

		template <typename U, typename D>
		constexpr friend auto operator<=>(const queue<U, D> &lhs, const queue<U, D> &rhs);
	};

	// Deduction guides
	// https://en.cppreference.com/w/cpp/container/queue/deduction_guides

	template <typename S>
	queue(S) -> queue<typename S::value_type, S>;

	template <typename S, typename A>
	queue(S, A) -> queue<typename S::value_type, S>;

	template <typename Iter>
	queue(Iter, Iter) -> queue<typename std::iterator_traits<Iter>::value_type>;

	template <typename Iter, typename A>
	queue(Iter, Iter, A) -> queue<typename std::iterator_traits<Iter>::value_type, std::list<typename std::iterator_traits<Iter>::value_type, A>>; // TODO replace with deque

	template <typename T, typename S>
	[[nodiscard]] constexpr inline bool operator==(const queue<T, S> &lhs, const queue<T, S> &rhs) {
		return lhs._sequence == rhs._sequence;
	}

	template <typename T, typename S>
	[[nodiscard]] constexpr inline auto operator<=>(const queue<T, S> &lhs, const queue<T, S> &rhs) {
		return lhs._sequence <=> rhs._sequence;
	}

	template <typename T, typename S>
	constexpr void swap(queue<T, S> &lhs, queue<T, S> &rhs) {
		lhs.swap(rhs);
	}

	// TODO uses_allocator<std::queue> helper
}