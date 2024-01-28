/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Provides access to the Real Time Clock
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/cmos.h>
#include <kernel/arch/x86_64/time/rtc.h>
#include <kernel/debug.h>

#define RTC_SECONDS_REG 0x00
#define RTC_MINUTES_REG 0x02
#define RTC_HOURS_REG 0x04
#define RTC_DAY_REG 0x07
#define RTC_MONTH_REG 0x08
#define RTC_YEAR_REG 0x09
#define RTC_STATUS_REG_A 0x0A
#define RTC_STATUS_REG_B 0x0B
#define RTC_STATUS_UPDATING 0x80
#define RTC_STATUS_12H 0x02
#define RTC_STATUS_BINARY 0x04
#define RTC_HOUR_PM 0x80

using namespace Time;

static DateTime _boot_time;

/**
 * @brief Convert a Binary Coded Decimal number to a binary number
 *
 * @param bcd The Binary Coded Decimal number
 * @return The binary number
 */
static uint8_t __bcd_to_binary(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
 * @brief Attempt to read the RTC
 *
 * @return The current time
 */
static DateTime __try_read_rtc(void) {
	DateTime dt;

	while (CMOS::read(RTC_STATUS_REG_A) & RTC_STATUS_UPDATING) {
	}
	// TODO timeout after some time

	dt.second = CMOS::read(RTC_SECONDS_REG);
	dt.minute = CMOS::read(RTC_MINUTES_REG);
	dt.hour = CMOS::read(RTC_HOURS_REG);
	dt.day = CMOS::read(RTC_DAY_REG);
	dt.month = CMOS::read(RTC_MONTH_REG);
	dt.year = CMOS::read(RTC_YEAR_REG);

	// TODO chech if century register is available

	return dt;
}

void RTC::init(void) {
	Debug::log("Initializing RTC...");
	_boot_time = now();
	Debug::log_info("Current time: %02d/%02d/%d - %02d:%02d:%02d",
					_boot_time.day,
					_boot_time.month,
					_boot_time.year,
					_boot_time.hour,
					_boot_time.minute,
					_boot_time.second);
}

DateTime RTC::now(void) {
	DateTime last;
	DateTime now = __try_read_rtc();

	do {
		last = now;
		now = __try_read_rtc();
	} while (last != now);
	// TODO timeout after some time

	uint8_t status = CMOS::read(RTC_STATUS_REG_B);
	bool is_pm = now.hour & RTC_HOUR_PM;
	now.hour &= ~RTC_HOUR_PM;

	if (!(status & RTC_STATUS_BINARY)) {
		now.second = __bcd_to_binary(now.second);
		now.minute = __bcd_to_binary(now.minute);
		now.hour = __bcd_to_binary(now.hour);
		now.day = __bcd_to_binary(now.day);
		now.month = __bcd_to_binary(now.month);
		now.year = __bcd_to_binary(now.year);
	}

	if (status & RTC_STATUS_12H) {
		now.hour = (now.hour % 12) + (is_pm ? 12 : 0);
	}

	now.year += 2000;
	return now;
}

DateTime RTC::boot_time(void) {
	return _boot_time;
}