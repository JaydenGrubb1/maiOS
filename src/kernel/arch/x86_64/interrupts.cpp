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
#include <lib/stdio.h>
#include <lib/string.h>
#include <stdint.h>

#define KERNEL_CODE_SEGMENT 0x08

#define GATE_TYPE_INTERRUPT 0xE
#define GATE_TYPE_TRAP 0xF
#define DPL_KERNEL 0x0 << 5
#define DPL_USER 0x3 << 5
#define PRESENT 0x1 << 7

struct IDTR {
	uint16_t size;
	uint64_t offset;
} __attribute__((packed));

struct IDTEntry {
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
} __attribute__((packed));

__attribute__((aligned(16))) static IDTEntry idt[256];
static IDTR idtr;

// 0: #DE - Division Error
extern "C" __attribute__((interrupt)) void division_error(Interrupts::StackFrame *frame) {
	Debug::log_failure("Division error");
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 1: #DB - Debug
extern "C" __attribute__((interrupt)) void debug(Interrupts::StackFrame *frame) {
	uint64_t dr6;
	asm volatile("mov %0, dr6"
				 : "=g"(dr6));

	// TODO Implement this properly
	// idk if this will ever be used

	if (dr6 && 0xf) {
		Debug::log_warning("Debug interrupt");
	} else {
		Debug::log_failure("Debug interrupt");
	}

	Debug::trace_stack(__builtin_frame_address(0));
	CPU::halt();
}

// 2: NMI - Non-maskable Interrupt
extern "C" __attribute__((interrupt)) void non_maskable(Interrupts::StackFrame *frame) {
	Debug::log_failure("Non-maskable interrupt");
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 3: #BP - Breakpoint
extern "C" __attribute__((interrupt)) void breakpoint(Interrupts::StackFrame *frame) {
	Debug::log_warning("Breakpoint interrupt");
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
}

// 4: #OF - Overflow
extern "C" __attribute__((interrupt)) void overflow(Interrupts::StackFrame *frame) {
	Debug::log_warning("Overflow exception");
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
}

// 6: #UD - Invalid Opcode
extern "C" __attribute__((interrupt)) void invalid_opcode(Interrupts::StackFrame *frame) {
	Debug::log_failure("Invalid opcode");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 7: #NM - Device Not Available
extern "C" __attribute__((interrupt)) void device_not_available(Interrupts::StackFrame *frame) {
	Debug::log_failure("Device not available");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 8: #DF - Double Fault
extern "C" __attribute__((interrupt)) void double_fault(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Double fault");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// VERIFY error_code is always 0
	// TODO implement error handling
	CPU::halt();
}

// 10: #TS - Invalid TSS
extern "C" __attribute__((interrupt)) void invalid_tss(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Invalid TSS: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 11: #NP - Segment Not Present
extern "C" __attribute__((interrupt)) void segment_not_present(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Segment not present: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 12: #SS - Stack Segment Fault
extern "C" __attribute__((interrupt)) void stack_segment_fault(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Stack segment fault: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 13: #GP - General Protection Fault
extern "C" __attribute__((interrupt)) void general_protection_fault(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("General protection fault: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 14: #PF - Page Fault
extern "C" __attribute__((interrupt)) void page_fault(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Page fault");

	printf("Error Code (%#.8lx):\n", error_code);
	printf("    P[0]: %c  W/R[1]: %c  U/S[2]: %c\n",
		   error_code & (1 << 0) ? '1' : '0', error_code & (1 << 1) ? '1' : '0', error_code & (1 << 2) ? '1' : '0');
	printf(" RSVD[3]: %c  I/D[4]: %c   PK[5]: %c\n",
		   error_code & (1 << 3) ? '1' : '0', error_code & (1 << 4) ? '1' : '0', error_code & (1 << 5) ? '1' : '0');
	printf("   SS[6]: %c HLAT[7]: %c SGK[15]: %c\n",
		   error_code & (1 << 6) ? '1' : '0', error_code & (1 << 7) ? '1' : '0', error_code & (1 << 15) ? '1' : '0');

	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 16: #MF - x87 Floating Point Exception
extern "C" __attribute__((interrupt)) void fpu_floating_point_error(Interrupts::StackFrame *frame) {
	Debug::log_failure("FPU floating point error");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 17: #AC - Alignment Check
extern "C" __attribute__((interrupt)) void alignment_check(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Alignment check");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// VERIFY error_code is always 0 except bit 0 (external event)
	// TODO implement error handling
	CPU::halt();
}

// 18: #MC - Machine Check
extern "C" __attribute__((interrupt)) void machine_check(Interrupts::StackFrame *frame) {
	Debug::log_failure("Machine check");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 19: #XM - SIMD Floating Point Exception
extern "C" __attribute__((interrupt)) void simd_floating_point_error(Interrupts::StackFrame *frame) {
	Debug::log_failure("SIMD floating point error");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 20: #VE - Virtualization Exception
extern "C" __attribute__((interrupt)) void virtualization_error(Interrupts::StackFrame *frame) {
	Debug::log_failure("Virtualization error");
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

// 21: #CP - Control Protection Exception
extern "C" __attribute__((interrupt)) void control_protection_exception(Interrupts::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Control protection exception: %lx", error_code);
	Interrupts::dump_stack_frame(frame);
	Debug::trace_stack(__builtin_frame_address(0));
	// TODO implement error handling
	CPU::halt();
}

extern "C" __attribute__((interrupt)) void default_isr(__attribute__((unused)) Interrupts::StackFrame *frame) {
	// do nothing
}

static void set_idt(uint8_t vector, uint64_t isr, uint8_t flags) {
	IDTEntry *entry = &idt[vector];
	memset(entry, 0, sizeof(IDTEntry));

	entry->offset_low = isr & 0xFFFF;
	entry->selector = KERNEL_CODE_SEGMENT;
	entry->ist = 0;
	((uint8_t *)entry)[5] = flags & 0xEF;
	entry->offset_mid = (isr >> 16) & 0xFFFF;
	entry->offset_high = (isr >> 32) & 0xFFFFFFFF;
}

bool Interrupts::clear_isr(uint8_t vector) {
	if (vector < 32) {
		Debug::log_failure("Cannot clear ISR for reserved vector %#x", vector);
		return false;
	}

	set_idt(vector, (uint64_t)default_isr, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	return true;
}

bool Interrupts::contains_isr(uint8_t vector) {
	if (vector < 32) {
		return true;
	}

	uint16_t default_low = ((uint64_t)default_isr) & 0xFFFF;
	uint16_t default_mid = (((uint64_t)default_isr) >> 16) & 0xFFFF;
	uint32_t default_high = (((uint64_t)default_isr) >> 32) & 0xFFFFFFFF;

	IDTEntry *entry = &idt[vector];
	return entry->offset_low != default_low ||
		   entry->offset_mid != default_mid ||
		   entry->offset_high != default_high;
}

void Interrupts::dump_stack_frame(Interrupts::StackFrame *frame) {
	printf("Stack Frame:\n");
	printf("    RIP: %#.16lx CS: %#.4lx\n", frame->rip, frame->cs);
	printf("    RSP: %#.16lx SS: %#.4lx\n", frame->rsp, frame->ss);
	printf(" RFLAGS: %#.8lx\n", frame->rflags);
}

void Interrupts::init(void) {
	Debug::log("Initializing IDT...");

	idtr.size = sizeof(idt) - 1;
	idtr.offset = (uint64_t)&idt;

	Debug::log("Installing exception handlers...");
	set_idt(0, (uint64_t)division_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(1, (uint64_t)debug, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(2, (uint64_t)non_maskable, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(3, (uint64_t)breakpoint, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(4, (uint64_t)overflow, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(6, (uint64_t)invalid_opcode, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(7, (uint64_t)device_not_available, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(8, (uint64_t)double_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(10, (uint64_t)invalid_tss, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(11, (uint64_t)segment_not_present, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(12, (uint64_t)stack_segment_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(13, (uint64_t)general_protection_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(14, (uint64_t)page_fault, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(16, (uint64_t)fpu_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(17, (uint64_t)alignment_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(18, (uint64_t)machine_check, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(19, (uint64_t)simd_floating_point_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(20, (uint64_t)virtualization_error, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	set_idt(21, (uint64_t)control_protection_exception, (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));

	Debug::log("Installing default interrupt handlers...");
	for (uint16_t vector = 32; vector < 256; vector++) {
		set_idt(vector, (uint64_t)default_isr, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	}

	Debug::log("Loading IDT...");
	asm volatile("lidt %0"
				 :
				 : "m"(idtr));

	Debug::log_ok("IDT initialized");
}

bool Interrupts::set_isr(uint8_t vector, void (*handler)(Interrupts::StackFrame *frame)) {
	if (vector < 32) {
		Debug::log_failure("Cannot set ISR for reserved vector %#x", vector);
		return false;
	}
	if (handler == nullptr) {
		Debug::log_failure("Cannot set ISR to null, use clear_isr() instead");
		return false;
	}
	if (contains_isr(vector)) {
		Debug::log_failure("ISR %#x already set, use clear_isr() first", vector);
		return false;
	}

	set_idt(vector, (uint64_t)handler, (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	return true;
}