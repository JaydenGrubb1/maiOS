/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-05
 * @brief Adaptor class template providing priority access to a data structure
 * @link https://en.cppreference.com/w/cpp/container/priority_queue @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <bits/algo_heap.h>
#include <bits/iterator_traits.h>
#include <functional>
#include <utility>
#include <vector>

namespace std {
	template <typename T, typename S = vector<T>, typename C = less<T>>
	class priority_queue {
	  public:
		using container_type = S;
		using value_compare = C;
		using value_type = typename S::value_type;
		using size_type = typename S::size_type;
		using reference = typename S::reference;
		using const_reference = typename S::const_reference;

	  private:
		S _sequence;
		C _compare;

	  public:
#pragma region Constructors
		constexpr priority_queue(void)
			requires(std::is_default_constructible_v<S> && std::is_default_constructible_v<C>)
			: _sequence(), _compare() {}

		constexpr explicit priority_queue(const C &compare)
			requires(std::is_default_constructible_v<S>)
			: _sequence(), _compare(compare) {}

		constexpr explicit priority_queue(const C &compare, const S &other)
			: _sequence(other), _compare(compare) {
			std::make_heap(_sequence.begin(), _sequence.end(), _compare);
		}

		constexpr explicit priority_queue(const C &compare, S &&other)
			: _sequence(std::move(other)), _compare(compare) {
			std::make_heap(_sequence.begin(), _sequence.end(), _compare);
		}

		constexpr priority_queue(const priority_queue &other)
			: _sequence(other._sequence), _compare(other._compare) {}

		constexpr priority_queue(priority_queue &&other)
			: _sequence(std::move(other._sequence)), _compare(std::move(other._compare)) {}

// TODO iterator constructor

// TODO allocator aware constructors (requires std::uses_allocator)
#pragma endregion

		[[nodiscard]] constexpr bool empty(void) const {
			return _sequence.empty();
		}

		[[nodiscard]] constexpr size_type size(void) const {
			return _sequence.size();
		}

		[[nodiscard]] const T &top(void) const {
			return _sequence.front();
		}

		constexpr void push(const T &value) {
			_sequence.push_back(value);
			std::push_heap(_sequence.begin(), _sequence.end(), _compare);
		}

		constexpr void push(T &&value) {
			_sequence.push_back(std::move(value));
			std::push_heap(_sequence.begin(), _sequence.end(), _compare);
		}

		template <typename... Args>
		constexpr void emplace(Args &&...args) {
			_sequence.emplace_back(std::forward<Args>(args)...);
			std::push_heap(_sequence.begin(), _sequence.end(), _compare);
		}

		constexpr void pop(void) {
			std::pop_heap(_sequence.begin(), _sequence.end(), _compare);
			_sequence.pop_back();
		}

		constexpr void swap(priority_queue &other) {
			using std::swap;
			swap(_sequence, other._sequence);
			swap(_compare, other._compare);
		}
	};

	// Deduction guides
	// https://en.cppreference.com/w/cpp/container/priority_queue/deduction_guides

	template <typename C, typename S>
	priority_queue(C, S) -> priority_queue<typename S::value_type, S, C>;

	template <typename Iter, typename C = less<typename std::iterator_traits<Iter>::value_type>, typename S = vector<typename std::iterator_traits<Iter>::value_type>>
	priority_queue(Iter, Iter, C = C(), S = S()) -> priority_queue<typename std::iterator_traits<Iter>::value_type, S, C>;

	template <typename C, typename S, typename A>
	priority_queue(C, S, A) -> priority_queue<typename S::value_type, S, C>;

	template <typename Iter, typename A>
	priority_queue(Iter, Iter, A) -> priority_queue<typename std::iterator_traits<Iter>::value_type, std::vector<typename std::iterator_traits<Iter>::value_type, A>, std::less<typename std::iterator_traits<Iter>::value_type>>;

	template <typename Iter, typename C, typename A>
	priority_queue(Iter, Iter, C, A) -> priority_queue<typename std::iterator_traits<Iter>::value_type, std::vector<typename std::iterator_traits<Iter>::value_type, A>, C>;

	template <typename Iter, typename C, typename S, typename A>
	priority_queue(Iter, Iter, C, S, A) -> priority_queue<typename std::iterator_traits<Iter>::value_type, S, C>;

	// TODO tidy up deduction guides

	// TODO equality operator ????

	template <typename T, typename S, typename C>
	constexpr void swap(priority_queue<T, S, C> &lhs, priority_queue<T, S, C> &rhs) {
		lhs.swap(rhs);
	}

	// TODO uses_allocator<std::priority_queue> helper
}