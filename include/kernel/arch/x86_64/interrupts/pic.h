/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-10
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

namespace PIC {
	/**
	 * @brief Sends an end of interrupt signal to the PIC
	 *
	 * @param irq The IRQ to send the EOI to
	 */
	void eoi(uint8_t irq);

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
}