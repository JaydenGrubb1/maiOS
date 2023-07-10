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

#include <kernel/arch/x86_64/interrupts.hpp>
#include <kernel/arch/x86_64/io.hpp>
#include <kernel/kprintf.hpp>
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
	kprintf("\u001b[31mexception: undefined\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt, aligned(16))) void spurious_interrupt(void *frame) {
	kprintf("\u001b[31mspurious interrupt: ignoring\u001b[0m\n");
}

extern "C" __attribute__((interrupt)) void division_error(void *frame) {
	kprintf("\u001b[31mexception: division error\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void debug(void *frame) {
	kprintf("\u001b[31mexception: debug\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void non_maskable(void *frame) {
	kprintf("\u001b[31mexception: non-maskable interrupt\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void breakpoint(void *frame) {
	kprintf("\u001b[31mexception: breakpoint\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void overflow(void *frame) {
	kprintf("\u001b[31mexception: overflow\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void bound_range_exceeded(void *frame) {
	kprintf("\u001b[31mexception: bound range exceeded\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void invalid_opcode(void *frame) {
	kprintf("\u001b[31mexception: invalid opcode\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void device_not_available(void *frame) {
	kprintf("\u001b[31mexception: device not available\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void double_fault(void *frame) {
	kprintf("\u001b[31mexception: double fault\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void invalid_tss(void *frame) {
	kprintf("\u001b[31mexception: invalid tss\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void segment_not_present(void *frame) {
	kprintf("\u001b[31mexception: segment not present\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void stack_segment_fault(void *frame) {
	kprintf("\u001b[31mexception: stack segment fault\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void general_protection_fault(void *frame) {
	kprintf("\u001b[31mexception: general protection fault\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void page_fault(void *frame) {
	kprintf("\u001b[31mexception: page fault\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void fpu_floating_point_error(void *frame) {
	kprintf("\u001b[31mexception: fpu floating point error\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void alignment_check(void *frame) {
	kprintf("\u001b[31mexception: alignment check\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void machine_check(void *frame) {
	kprintf("\u001b[31mexception: machine check\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void simd_floating_point_error(void *frame) {
	kprintf("\u001b[31mexception: simd floating point error\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void virtualization_error(void *frame) {
	kprintf("\u001b[31mexception: virtualization error\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute__((interrupt)) void control_protection_exception(void *frame) {
	kprintf("\u001b[31mexception: control protection exception\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void hypervisor_injection_expection(void *frame) {
	kprintf("\u001b[31mexception: hypervisor injection exception\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void vmm_communication_expectation(void *frame) {
	kprintf("\u001b[31mexception: vmm communication exception\u001b[0m\n");
	// TODO implement error handling
	asm volatile("cli; hlt");
}

extern "C" __attribute((interrupt)) void security_exception(void *frame) {
	kprintf("\u001b[31mexception: security exception\u001b[0m\n");
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
	kprintf("> initializing IDT...\n");

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	kprintf("> installing exception handlers...\n");
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

	kprintf("> installing default interrupt handlers...\n");
	for (uint16_t vector = 32; vector < 256; vector++) {
		set_idt(vector, default_isr, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	}

	kprintf("> loading IDT...\n");
	asm volatile("lidt %0"
				 :
				 : "m"(idtr));

	kprintf("> IDT initialized!\n");
}