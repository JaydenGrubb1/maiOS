/**
 * @file include/kernel/uart.hpp
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-24
 * @brief // DOC
 *
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define UART_DATA_5_BITS 0x00
#define UART_DATA_6_BITS 0x01
#define UART_DATA_7_BITS 0x02
#define UART_DATA_8_BITS 0x03
#define UART_STOP_1_BITS 0x00
#define UART_STOP_2_BITS 0x04
#define UART_PARITY_NONE 0x00
#define UART_PARITY_ODD 0x08
#define UART_PARITY_EVEN 0x18
#define UART_PARITY_MARK 0x28
#define UART_PARITY_SPACE 0x38

#define UART_OFFSET_LINE_CONTROL 3
#define UART_ENABLE_DLAB 0x80
#define UART_MAX_BAUD_RATE 115200

#include <stdbool.h>
#include <stdint.h>

class UART {
  public:
	enum UARTPort : uint16_t {
		COM1 = 0x3F8,
		COM2 = 0x2F8,
		COM3 = 0x3E8,
		COM4 = 0x2E8
	};

	UART(UARTPort port);
	UART(const UART &) = delete;
	~UART();
	UARTPort getPort();
	bool setBaudRate(uint32_t rate);
	bool setLineProtocol(uint8_t protocol);
	uint8_t read();
	void write(uint8_t value);

  private:
	UARTPort port_m;
	uint32_t baudRate_m;
	uint8_t protocol_m;
	static bool portUsed_m[4];
};