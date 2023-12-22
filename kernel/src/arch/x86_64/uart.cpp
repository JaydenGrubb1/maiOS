/**
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

#include <stdbool.h>
#include <stdint.h>

#include <arch/x86_64/io.h>
#include <arch/x86_64/uart.h>

// FIXME Get this decleration inside uart.h
bool UART::portUsed_m[4];

uint16_t portToIndex(UART::UARTPort port) {
	return ((port - 744) % 7) / 2;
}

UART::UART(UARTPort port) : port_m(port) {
	// TODO
	// IO::out8(port + 1, 0x00); // Disable all interupts

	setBaudRate(UART_MAX_BAUD_RATE);
	setLineProtocol(UART_DATA_8_BITS | UART_PARITY_NONE | UART_STOP_1_BITS);

	// TODO
	// IO::out8(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	// IO::out8(port + 4, 0x0B); // IRQs enabled, RTS/DSR set

	portUsed_m[portToIndex(port)] = true;
}

UART::~UART() {
	// TODO Unsetup ports?
	portUsed_m[portToIndex(this->port_m)] = false;
}

UART::UARTPort UART::getPort() {
	return this->port_m;
}

bool UART::setBaudRate(uint32_t rate) {
	uint32_t divisor = UART_MAX_BAUD_RATE / rate;
	uint8_t lower = divisor & 0xFF;
	uint8_t upper = (divisor >> 8) & 0xFF;
	IO::out8(this->port_m + UART_OFFSET_LINE_CONTROL, UART_ENABLE_DLAB);
	IO::out8(this->port_m + 0, lower);
	IO::out8(this->port_m + 1, upper);
	// TODO Disable DLAB

	this->baudRate_m = rate;
	return true;
}

bool UART::setLineProtocol(uint8_t protocol) {
	// TODO Perform checks
	IO::out8(this->port_m + UART_OFFSET_LINE_CONTROL, protocol);

	this->protocol_m = protocol;
	return true;
}

uint8_t UART::read() {
	while ((IO::in8(this->port_m + 5) & 0x01) == 0) {
		// TODO Wait
	}
	return IO::in8(this->port_m);
}

void UART::write(uint8_t value) {
	while ((IO::in8(this->port_m + 5) & 0x20) == 0) {
		// TODO Wait
	}
	IO::out8(this->port_m, value);
}