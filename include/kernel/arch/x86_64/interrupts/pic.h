/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-10
 * @brief Various functions for interacting with the Programmable Interrupt Controller
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

namespace PIC {
	/**
	 * @brief Initializes the Programmable Interrupt Controller
	 *
	 */
	void init(void);

	/**
	 * @brief Remaps the PIC to the specified master and slave IRQs
	 *
	 * @param master The IRQ to use for the master PIC
	 * @param slave The IRQ to use for the slave PIC
	 */
	void remap(uint8_t master, uint8_t slave);

	/**
	 * @brief Sends an end of interrupt signal to the PIC
	 *
	 * @param irq The IRQ to send the EOI to
	 */
	void eoi(uint8_t irq);

	/**
	 * @brief Sets the mask for the specified IRQ,
	 * i.e. disables the PIC from sending interrupts for the IRQ
	 *
	 * @param irq The IRQ to set the mask for
	 * @return true if the mask was set successfully
	 */
	bool set_mask(uint8_t irq);

	/**
	 * @brief Clears the mask for the specified IRQ,
	 * i.e. enables the PIC to send interrupts for the IRQ
	 *
	 * @param irq The IRQ to clear the mask for
	 * @return true if the mask was cleared successfully
	 */
	bool clear_mask(uint8_t irq);

	/**
	 * @brief Disables the Programmable Interrupt Controller
	 *
	 */
	void disable(void);
}