set(ASM_SOURCES
	boot/header.asm
	boot/init32.asm
	boot/init64.asm
	# crt/crti.s
	# crt/crtn.s
)

set(CPP_SOURCES
	interrupts/apic.cpp
	interrupts/pic.cpp
	time/pit.cpp
	time/rtc.cpp
	cmos.cpp
	interrupts.cpp
	kmain.cpp
	ksyms.cpp
	multiboot2.cpp
	uart.cpp
)

set(KERNEL_ARCH_SOURCES
	${ASM_SOURCES}
	${CPP_SOURCES}
)

list(TRANSFORM KERNEL_ARCH_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(KERNEL_ARCH_SOURCES ${KERNEL_ARCH_SOURCES} PARENT_SCOPE)