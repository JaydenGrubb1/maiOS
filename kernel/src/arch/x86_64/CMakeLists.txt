set(ASM_SOURCES
	asm/boot.asm
	asm/scheduler.asm
)

set(CPP_SOURCES
	interrupts/apic.cpp
	interrupts/pic.cpp
	memory/page_table.cpp
	memory/paging.cpp
	memory/physical_memory.cpp
	time/pit.cpp
	time/rtc.cpp
	acpi.cpp
	cmos.cpp
	cpu.cpp
	framebuffer.cpp
	interrupts.cpp
	ksyms.cpp
	main.cpp
	memory.cpp
	multiboot2.cpp
	scheduler.cpp
	uart.cpp
)

set(KERNEL_ARCH_SOURCES
	${ASM_SOURCES}
	${CPP_SOURCES}
)

list(TRANSFORM KERNEL_ARCH_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(KERNEL_ARCH_SOURCES ${KERNEL_ARCH_SOURCES} PARENT_SCOPE)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld PARENT_SCOPE)