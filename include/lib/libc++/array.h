/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-29
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __is_kernel
#include <lib/libc++/optional.h>
#endif

#include <stddef.h>

namespace kstd {
	template <typename T, size_t N>
	class array {
	  public:
		T _data[N];

		constexpr size_t size() const {
			return N;
		}

		constexpr size_t max_size() const {
			return N;
		}

		constexpr bool empty() const {
			return N == 0;
		}

		constexpr T &operator[](size_t index) {
			return _data[index];
		}

		constexpr const T &operator[](size_t index) const {
			return _data[index];
		}

		constexpr optional<T> at(size_t index) {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		constexpr optional<const T> at(size_t index) const {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		constexpr T &front() {
			return _data[0];
		}

		constexpr const T &front() const {
			return _data[0];
		}

		constexpr T &back() {
			return _data[N - 1];
		}

		constexpr const T &back() const {
			return _data[N - 1];
		}

		constexpr T *data() {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		constexpr const T *data() const {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		constexpr T *begin() {
			return _data;
		}

		constexpr const T *begin() const {
			return _data;
		}

		constexpr const T *cbegin() const {
			return _data;
		}

		constexpr T *end() {
			return _data + size();
		}

		constexpr const T *end() const {
			return _data + size();
		}

		constexpr const T *cend() const {
			return _data + size();
		}

		// TODO Add rbegin, rend, crbegin, crend

		void fill(const T &value) {
			// VERIFY Should this use std::move()
			for (size_t i = 0; i < size(); i++) {
				_data[i] = value;
			}
		}

		void swap(array &other) {
			// VERIFY Should this use std::move()
			for (size_t i = 0; i < size(); i++) {
				T temp = _data[i];
				_data[i] = other._data[i];
				other._data[i] = temp;
			}
		}
	};
}