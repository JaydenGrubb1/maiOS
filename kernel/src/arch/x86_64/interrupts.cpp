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

#include <stdint.h>

#include <cstring>

#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/debug.h>
#include <kernel/defines.h>
#include <kernel/panic.h>

#define INTERRUPT __attribute__((interrupt))
#define KERNEL_CODE_SEGMENT 0x08

#define GATE_TYPE_INTERRUPT 0xE
#define GATE_TYPE_TRAP 0xF
#define DPL_KERNEL 0x0 << 5
#define DPL_USER 0x3 << 5
#define PRESENT 0x1 << 7

struct IDTR {
	uint16_t size;
	uint64_t offset;
} PACKED;

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
} PACKED;

static ALIGNED(16) IDTEntry idt[256];
static IDTR idtr;

// 0: #DE - Division Error
extern "C" INTERRUPT void division_error(CPU::StackFrame *frame) {
	Debug::log_failure("Division error");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Division Error)");
}

// 1: #DB - Debug
extern "C" INTERRUPT void debug(UNUSED CPU::StackFrame *frame) {
	uint64_t dr6;
	asm volatile("mov %0, dr6"
				 : "=g"(dr6));

	if (dr6 && 0xf) {
		Debug::log_warning("Debug interrupt");
	} else {
		Debug::log_failure("Debug interrupt");
	}

	// TODO Implement this properly
	// idk if this will ever be used

	Kernel::panic("Unhandled Exception (Debug)");
}

// 2: NMI - Non-maskable Interrupt
extern "C" INTERRUPT void non_maskable(UNUSED CPU::StackFrame *frame) {
	Debug::log_failure("Non-maskable interrupt");
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Non-maskable Interrupt)");
}

// 3: #BP - Breakpoint
extern "C" INTERRUPT void breakpoint(UNUSED CPU::StackFrame *frame) {
	Debug::log_warning("Breakpoint interrupt");
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Breakpoint)");
}

// 4: #OF - Overflow
extern "C" INTERRUPT void overflow(UNUSED CPU::StackFrame *frame) {
	Debug::log_warning("Overflow exception");
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Overflow)");
}

// 6: #UD - Invalid Opcode
extern "C" INTERRUPT void invalid_opcode(CPU::StackFrame *frame) {
	Debug::log_failure("Invalid opcode");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Invalid Opcode)");
}

// 7: #NM - Device Not Available
extern "C" INTERRUPT void device_not_available(CPU::StackFrame *frame) {
	Debug::log_failure("Device not available");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Device Not Available)");
}

// 8: #DF - Double Fault
extern "C" INTERRUPT void double_fault(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Double fault: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// VERIFY error_code is always 0
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Double Fault)");
}

// 10: #TS - Invalid TSS
extern "C" INTERRUPT void invalid_tss(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Invalid TSS: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Invalid TSS)");
}

// 11: #NP - Segment Not Present
extern "C" INTERRUPT void segment_not_present(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Segment not present: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Segment Not Present)");
}

// 12: #SS - Stack Segment Fault
extern "C" INTERRUPT void stack_segment_fault(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Stack segment fault: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Stack Segment Fault)");
}

// 13: #GP - General Protection Fault
extern "C" INTERRUPT void general_protection_fault(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("General protection fault: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (General Protection Fault)");
}

// 14: #PF - Page Fault
extern "C" INTERRUPT void page_fault(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Page fault");

	Debug::log_raw("Error Code (%#.8lx):\n", error_code);
	Debug::log_raw("    P[0]: %c  W/R[1]: %c  U/S[2]: %c\n",
				   error_code & (1 << 0) ? '1' : '0', error_code & (1 << 1) ? '1' : '0', error_code & (1 << 2) ? '1' : '0');
	Debug::log_raw(" RSVD[3]: %c  I/D[4]: %c   PK[5]: %c\n",
				   error_code & (1 << 3) ? '1' : '0', error_code & (1 << 4) ? '1' : '0', error_code & (1 << 5) ? '1' : '0');
	Debug::log_raw("   SS[6]: %c HLAT[7]: %c SGK[15]: %c\n",
				   error_code & (1 << 6) ? '1' : '0', error_code & (1 << 7) ? '1' : '0', error_code & (1 << 15) ? '1' : '0');

	Interrupts::dump_stack_frame(frame);

	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Page Fault)");
}

// 16: #MF - x87 Floating Point Exception
extern "C" INTERRUPT void fpu_floating_point_error(CPU::StackFrame *frame) {
	Debug::log_failure("FPU floating point error");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (FPU Floating Point Error)");
}

// 17: #AC - Alignment Check
extern "C" INTERRUPT void alignment_check(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Alignment check: %lu", error_code);
	Interrupts::dump_stack_frame(frame);
	// VERIFY error_code is always 0 except bit 0 (external event)
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Alignment Check)");
}

// 18: #MC - Machine Check
extern "C" INTERRUPT void machine_check(CPU::StackFrame *frame) {
	Debug::log_failure("Machine check");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Machine Check)");
}

// 19: #XM - SIMD Floating Point Exception
extern "C" INTERRUPT void simd_floating_point_error(CPU::StackFrame *frame) {
	Debug::log_failure("SIMD floating point error");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (SIMD Floating Point Error)");
}

// 20: #VE - Virtualization Exception
extern "C" INTERRUPT void virtualization_error(CPU::StackFrame *frame) {
	Debug::log_failure("Virtualization error");
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Virtualization Error)");
}

// 21: #CP - Control Protection Exception
extern "C" INTERRUPT void control_protection_exception(CPU::StackFrame *frame, uint64_t error_code) {
	Debug::log_failure("Control protection exception: %lx", error_code);
	Interrupts::dump_stack_frame(frame);
	// TODO implement error handling
	Kernel::panic("Unhandled Exception (Control Protection Exception)");
}

extern "C" INTERRUPT void default_isr(UNUSED CPU::StackFrame *frame) {
	// do nothing
}

static void __set_idt(uint8_t vector, void *isr, uint8_t flags) {
	IDTEntry *entry = &idt[vector];
	memset(entry, 0, sizeof(IDTEntry));

	entry->offset_low = reinterpret_cast<uintptr_t>(isr) & 0xFFFF;
	entry->selector = KERNEL_CODE_SEGMENT;
	entry->ist = 0;
	reinterpret_cast<uint8_t *>(entry)[5] = flags & 0xEF;
	entry->offset_mid = (reinterpret_cast<uintptr_t>(isr) >> 16) & 0xFFFF;
	entry->offset_high = (reinterpret_cast<uintptr_t>(isr) >> 32) & 0xFFFFFFFF;
}

void Interrupts::dump_stack_frame(CPU::StackFrame *frame) {
	Debug::log_raw("Stack Frame:\n");
	Debug::log_raw("    RIP: %#.16lx CS: %#.4lx\n", frame->rip, frame->cs);
	Debug::log_raw("    RSP: %#.16lx SS: %#.4lx\n", frame->rsp, frame->ss);
	Debug::log_raw(" RFLAGS: %#.8lx\n", frame->rflags);
}

void Interrupts::init(void) {
	Debug::log("Initializing IDT...");

	idtr.size = sizeof(idt) - 1;
	idtr.offset = reinterpret_cast<uint64_t>(&idt);

	Debug::log("Installing exception handlers...");
	__set_idt(0, reinterpret_cast<void *>(division_error), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(1, reinterpret_cast<void *>(debug), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(2, reinterpret_cast<void *>(non_maskable), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(3, reinterpret_cast<void *>(breakpoint), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(4, reinterpret_cast<void *>(overflow), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(6, reinterpret_cast<void *>(invalid_opcode), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(7, reinterpret_cast<void *>(device_not_available), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(8, reinterpret_cast<void *>(double_fault), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(10, reinterpret_cast<void *>(invalid_tss), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(11, reinterpret_cast<void *>(segment_not_present), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(12, reinterpret_cast<void *>(stack_segment_fault), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(13, reinterpret_cast<void *>(general_protection_fault), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(14, reinterpret_cast<void *>(page_fault), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(16, reinterpret_cast<void *>(fpu_floating_point_error), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(17, reinterpret_cast<void *>(alignment_check), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(18, reinterpret_cast<void *>(machine_check), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(19, reinterpret_cast<void *>(simd_floating_point_error), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(20, reinterpret_cast<void *>(virtualization_error), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));
	__set_idt(21, reinterpret_cast<void *>(control_protection_exception), (GATE_TYPE_TRAP | DPL_KERNEL | PRESENT));

	Debug::log("Installing default interrupt handlers...");
	for (uint16_t vector = 32; vector < 256; vector++) {
		__set_idt(vector, reinterpret_cast<void *>(default_isr), (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	}

	Debug::log("Loading IDT...");
	asm volatile("lidt %0"
				 :
				 : "m"(idtr));

	Debug::log_ok("IDT initialized");
}

bool Interrupts::set_isr(uint8_t vector, void (*handler)(CPU::StackFrame *frame)) {
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

	__set_idt(vector, reinterpret_cast<void *>(handler), (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	return true;
}

bool Interrupts::clear_isr(uint8_t vector) {
	if (vector < 32) {
		Debug::log_failure("Cannot clear ISR for reserved vector %#x", vector);
		return false;
	}

	__set_idt(vector, reinterpret_cast<void *>(default_isr), (GATE_TYPE_INTERRUPT | DPL_KERNEL | PRESENT));
	return true;
}

bool Interrupts::contains_isr(uint8_t vector) {
	if (vector < 32) {
		return true;
	}

	// TODO find way to replace with constexpr
	uint16_t default_low = reinterpret_cast<uintptr_t>(default_isr) & 0xFFFF;
	uint16_t default_mid = (reinterpret_cast<uintptr_t>(default_isr) >> 16) & 0xFFFF;
	uint32_t default_high = (reinterpret_cast<uintptr_t>(default_isr) >> 32) & 0xFFFFFFFF;

	IDTEntry *entry = &idt[vector];
	return entry->offset_low != default_low ||
		   entry->offset_mid != default_mid ||
		   entry->offset_high != default_high;
}