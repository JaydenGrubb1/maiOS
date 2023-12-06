/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-10
 * @brief Handles all interrupt related tasks for the kernel
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <defines.h>
#include <stdbool.h>
#include <stdint.h>

namespace Interrupts {
	/**
	 * @brief Interrupt stack frame
	 *
	 */
	struct StackFrame {
		uint64_t rip;
		uint64_t cs;
		uint64_t rflags;
		uint64_t rsp;
		uint64_t ss;
	} PACKED;

	/**
	 * @brief Clears the interrupt flag
	 *
	 */
	ALWAYS_INLINE void disable(void) {
		asm volatile("cli");
	}

	/**
	 * @brief Sets the interrupt flag
	 *
	 */
	ALWAYS_INLINE void enable(void) {
		asm volatile("sti");
	}

	/**
	 * @brief Dump a stack frame to output
	 *
	 * @param frame The stack frame to dump
	 */
	void dump_stack_frame(StackFrame *frame);

	/**
	 * @brief Initializes the Interrupt Descriptor Table with the
	 * default ISR and exception handlers and then loads it
	 *
	 */
	void init(void);

	/**
	 * @brief Sets an interrupt service routine in the IDT
	 *
	 * @param vector The interrupt vector to set
	 * @param handler The interrupt service routine
	 * @return true if the ISR was set successfully
	 */
	bool set_isr(uint8_t vector, void (*handler)(StackFrame *frame));

	/**
	 * @brief Removes an interrupt service routine from the IDT
	 *
	 * @param vector The interrupt vector to clear
	 * @return true if the ISR was cleared successfully
	 */
	bool clear_isr(uint8_t vector);

	/**
	 * @brief Checks if an interrupt service routine is set in the IDT
	 *
	 * @param vector The interrupt vector to check
	 * @return true if the ISR is set
	 */
	bool contains_isr(uint8_t vector);
}