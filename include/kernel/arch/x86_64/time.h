/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-27
 * @brief Various time related functions
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

namespace Time {
	/**
	 * @brief A date and time
	 *
	 */
	struct DateTime {
		uint8_t second = 0;
		uint8_t minute = 0;
		uint8_t hour = 0;
		uint8_t day = 1;
		uint8_t month = 1;
		uint16_t year = 1970;

		/**
		 * @brief Compare two DateTime objects
		 *
		 * @param other The other DateTime object
		 * @return true if the two DateTime objects are equal
		 */
		bool operator==(const DateTime &other) const {
			return second == other.second && minute == other.minute && hour == other.hour && day == other.day && month == other.month && year == other.year;
		}
	};
}