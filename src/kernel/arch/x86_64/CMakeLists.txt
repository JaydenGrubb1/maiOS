set(ASM_SOURCES
	boot/header.asm
	boot/init32.asm
	boot/init64.asm
)

set(CPP_SOURCES
	interrupts/apic.cpp
	interrupts/pic.cpp
	time/pit.cpp
	time/rtc.cpp
	cmos.cpp
	interrupts.cpp
	ksyms.cpp
	main.cpp
	memory.cpp
	multiboot2.cpp
	uart.cpp
)

set(KERNEL_ARCH_SOURCES
	${ASM_SOURCES}
	${CPP_SOURCES}
)

list(TRANSFORM KERNEL_ARCH_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

set(KERNEL_ARCH_SOURCES ${KERNEL_ARCH_SOURCES} PARENT_SCOPE)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld PARENT_SCOPE)
set(CRTI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/crt/crti.asm PARENT_SCOPE)
set(CRTN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/crt/crtn.asm PARENT_SCOPE)