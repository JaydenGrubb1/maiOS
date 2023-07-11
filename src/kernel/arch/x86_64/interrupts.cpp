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

#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/io.h>
#include <kernel/logger.h>
#include <stdint.h>

#define GATE_TYPE_INTERRUPT 0xE
#define GATE_TYPE_TRAP 0xF
#define DPL_KERNEL 0x0 << 5
#define DPL_USER 0x3 << 5
#define PRESENT 0x1 << 7

typedef struct {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed)) idtr_t;

typedef struct {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t ist : 3;
	unsigned : 5;
	uint8_t gate_type : 4;
	unsigned : 1;
	uint8_t dpl : 2;
	uint8_t present : 1;
	uint16_t offset_mid;
	uint32_t offset_high;
	unsigned : 32;
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(16))) static idt_entry_t idt[256];
static idtr_t idtr;

extern "C" __attribute__((interrupt)) void undefined_exception(void *frame) {
	LOG_FAIL("Undefined exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt, aligned(16))) void spurious_interrupt(void *frame) {
	LOG_WARN("Spurious interrupt, ignoring");
}

extern "C" __attribute__((interrupt)) void division_error(void *frame) {
	LOG_FAIL("Division error");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void debug(void *frame) {
	LOG_WARN("Debug interrupt");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void non_maskable(void *frame) {
	LOG_FAIL("Non-maskable interrupt");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void breakpoint(void *frame) {
	LOG_WARN("Breakpoint interrupt");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void overflow(void *frame) {
	LOG_FAIL("Overflow exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void bound_range_exceeded(void *frame) {
	LOG_FAIL("Bound range exceeded");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void invalid_opcode(void *frame) {
	LOG_FAIL("Invalid opcode");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void device_not_available(void *frame) {
	LOG_FAIL("Device not available");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void double_fault(void *frame) {
	LOG_FAIL("Double fault");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void invalid_tss(void *frame) {
	LOG_FAIL("Invalid TSS");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void segment_not_present(void *frame) {
	LOG_FAIL("Segment not present");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void stack_segment_fault(void *frame) {
	LOG_FAIL("Stack segment fault");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void general_protection_fault(void *frame) {
	LOG_FAIL("General protection fault");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void page_fault(void *frame) {
	LOG_FAIL("Page fault");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void fpu_floating_point_error(void *frame) {
	LOG_FAIL("FPU floating point error");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void alignment_check(void *frame) {
	LOG_FAIL("Alignment check");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void machine_check(void *frame) {
	LOG_FAIL("Machine check");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void simd_floating_point_error(void *frame) {
	LOG_FAIL("SIMD floating point error");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void virtualization_error(void *frame) {
	LOG_FAIL("Virtualization error");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void control_protection_exception(void *frame) {
	LOG_FAIL("Control protection exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void hypervisor_injection_expection(void *frame) {
	LOG_FAIL("Hypervisor injection exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void vmm_communication_expectation(void *frame) {
	LOG_FAIL("VMM communication exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void security_exception(void *frame) {
	LOG_FAIL("Security exception");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void default_isr(void *frame) {
	// do nothing
}

void set_idt(uint8_t vector, void (*isr)(void *), uint8_t flags) {
	idt_entry_t *entry = &idt[vector];
	*entry = {0};

	entry->offset_low = (uint64_t)isr & 0xFFFF;
	entry->selector = 0x08;
	entry->ist = 0;
	((uint8_t *)entry)[5] = flags & 0xEF;
	entry->offset_mid = ((uint64_t)isr >> 16) & 0xFFFF;
	entry->offset_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
}

void Interrupts::init_idt(void) {
	LOG("Initializing IDT...");

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	LOG("Installing exception handlers...");
	set_idt(0, division_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(1, debug, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(2, non_maskable, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(3, breakpoint, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(4, overflow, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(5, bound_range_exceeded, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(6, invalid_opcode, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(7, device_not_available, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(8, double_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(9, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(10, invalid_tss, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(11, segment_not_present, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(12, stack_segment_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(13, general_protection_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(14, page_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(16, fpu_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(17, alignment_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(18, machine_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(19, simd_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(20, virtualization_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(21, control_protection_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(28, hypervisor_injection_expection, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(29, vmm_communication_expectation, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(30, security_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));

	LOG("Installing default interrupt handlers...");
	for (uint16_t vector = 32; vector < 256; vector++) {
		set_idt(vector, default_isr, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	}

	LOG("Loading IDT...");
	asm volatile("lidt %0"
				 :
				 : "m"(idtr));

	LOG_PASS("IDT initialized");
}