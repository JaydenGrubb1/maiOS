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

#include <kernel/arch/x86_64/cpu.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/io.h>
#include <kernel/debug.h>
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
	Debug::log_failure("Undefined exception");
	// TODO implement error handling
	CPU::halt();
}

extern "C" __attribute__((interrupt, aligned(16))) void spurious_interrupt(void *frame) {
	Debug::log_warning("Spurious interrupt, ignoring");
}

// 0: #DE - Division Error
extern "C" __attribute__((interrupt)) void division_error(void *frame) {
	Debug::log_failure("Division error");
	// TODO implement error handling
	CPU::halt();
}

// 1: #DB - Debug
extern "C" __attribute__((interrupt)) void debug(void *frame) {
	Debug::log_warning("Debug interrupt");
	// TODO implement error handling
	CPU::halt();
}

// 2: NMI - Non-maskable Interrupt
extern "C" __attribute__((interrupt)) void non_maskable(void *frame) {
	Debug::log_failure("Non-maskable interrupt");
	// TODO implement error handling
	CPU::halt();
}

// 3: #BP - Breakpoint
extern "C" __attribute__((interrupt)) void breakpoint(void *frame) {
	Debug::log_warning("Breakpoint interrupt");
	// TODO implement error handling
	CPU::halt();
}

// 4: #OF - Overflow
extern "C" __attribute__((interrupt)) void overflow(void *frame) {
	Debug::log_failure("Overflow exception");
	// TODO implement error handling
	CPU::halt();
}

// 5: #BR - Bound Range Exceeded
extern "C" __attribute__((interrupt)) void bound_range_exceeded(void *frame) {
	Debug::log_failure("Bound range exceeded");
	// TODO implement error handling
	CPU::halt();
}

// 6: #UD - Invalid Opcode
extern "C" __attribute__((interrupt)) void invalid_opcode(void *frame) {
	Debug::log_failure("Invalid opcode");
	// TODO implement error handling
	CPU::halt();
}

// 7: #NM - Device Not Available
extern "C" __attribute__((interrupt)) void device_not_available(void *frame) {
	Debug::log_failure("Device not available");
	// TODO implement error handling
	CPU::halt();
}

// 8: #DF - Double Fault
extern "C" __attribute__((interrupt)) void double_fault(void *frame) {
	Debug::log_failure("Double fault");
	// TODO implement error handling
	CPU::halt();
}

// 10: #TS - Invalid TSS
extern "C" __attribute__((interrupt)) void invalid_tss(void *frame) {
	Debug::log_failure("Invalid TSS");
	// TODO implement error handling
	CPU::halt();
}

// 11: #NP - Segment Not Present
extern "C" __attribute__((interrupt)) void segment_not_present(void *frame) {
	Debug::log_failure("Segment not present");
	// TODO implement error handling
	CPU::halt();
}

// 12: #SS - Stack Segment Fault
extern "C" __attribute__((interrupt)) void stack_segment_fault(void *frame) {
	Debug::log_failure("Stack segment fault");
	// TODO implement error handling
	CPU::halt();
}

// 13: #GP - General Protection Fault
extern "C" __attribute__((interrupt)) void general_protection_fault(void *frame) {
	Debug::log_failure("General protection fault");
	// TODO implement error handling
	CPU::halt();
}

// 14: #PF - Page Fault
extern "C" __attribute__((interrupt)) void page_fault(void *frame) {
	Debug::log_failure("Page fault");
	// TODO implement error handling
	CPU::halt();
}

// 16: #MF - x87 Floating Point Exception
extern "C" __attribute__((interrupt)) void fpu_floating_point_error(void *frame) {
	Debug::log_failure("FPU floating point error");
	// TODO implement error handling
	CPU::halt();
}

// 17: #AC - Alignment Check
extern "C" __attribute__((interrupt)) void alignment_check(void *frame) {
	Debug::log_failure("Alignment check");
	// TODO implement error handling
	CPU::halt();
}

// 18: #MC - Machine Check
extern "C" __attribute__((interrupt)) void machine_check(void *frame) {
	Debug::log_failure("Machine check");
	// TODO implement error handling
	CPU::halt();
}

// 19: #XM - SIMD Floating Point Exception
extern "C" __attribute__((interrupt)) void simd_floating_point_error(void *frame) {
	Debug::log_failure("SIMD floating point error");
	// TODO implement error handling
	CPU::halt();
}

// 20: #VE - Virtualization Exception
extern "C" __attribute__((interrupt)) void virtualization_error(void *frame) {
	Debug::log_failure("Virtualization error");
	// TODO implement error handling
	CPU::halt();
}

// 21: #CP - Control Protection Exception
extern "C" __attribute__((interrupt)) void control_protection_exception(void *frame) {
	Debug::log_failure("Control protection exception");
	// TODO implement error handling
	CPU::halt();
}

// 28: #VI - Hypervisor Injection Exception
extern "C" __attribute((interrupt)) void hypervisor_injection_expection(void *frame) {
	Debug::log_failure("Hypervisor injection exception");
	// TODO implement error handling
	CPU::halt();
}

// 29: #VC - VMM Communication Exception
extern "C" __attribute((interrupt)) void vmm_communication_expectation(void *frame) {
	Debug::log_failure("VMM communication exception");
	// TODO implement error handling
	CPU::halt();
}

// 30: #SX - Security Exception
extern "C" __attribute((interrupt)) void security_exception(void *frame) {
	Debug::log_failure("Security exception");
	// TODO implement error handling
	CPU::halt();
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
	Debug::log("Initializing IDT...");

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	Debug::log("Installing exception handlers...");
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
	// set_idt(15, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(16, fpu_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(17, alignment_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(18, machine_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(19, simd_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(20, virtualization_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(21, control_protection_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(22, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(23, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(24, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(25, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(26, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	// set_idt(27, undefined_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(28, hypervisor_injection_expection, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(29, vmm_communication_expectation, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(30, security_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));

	Debug::log("Installing default interrupt handlers...");
	for (uint16_t vector = 32; vector < 256; vector++) {
		set_idt(vector, default_isr, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	}

	Debug::log("Loading IDT...");
	asm volatile("lidt %0"
				 :
				 : "m"(idtr));

	Debug::log_ok("IDT initialized");
}