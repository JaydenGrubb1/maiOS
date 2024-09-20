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

#include <cstdint>

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/defines.h>

namespace Interrupts {
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
	 * @brief Invokes an interrupt vector
	 *
	 * @tparam vector The interrupt vector to invoke
	 */
	template <uint8_t vector>
	ALWAYS_INLINE void invoke(void) {
		asm volatile("int %0" ::"i"(vector));
	}

	/**
	 * @brief Checks if interrupts are enabled
	 *
	 * @return true if interrupts are enabled, false otherwise
	 */
	[[nodiscard]] inline bool is_enabled(void) {
		return CPU::get_flags() & 0x200;
	}

	/**
	 * @brief Dump a stack frame to output
	 *
	 * @param frame The stack frame to dump
	 */
	void dump_stack_frame(CPU::StackFrame *frame);

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
	bool set_isr(uint8_t vector, void (*handler)(CPU::StackFrame *frame));

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