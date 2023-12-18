set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/toolchain.cmake)

cmake_minimum_required(VERSION 3.16)
project(kernel)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)

set(ARCH x86_64)

include_directories(include)

set(QEMU_FLAGS
	-m 128M
	-serial stdio
	# -d int,cpu_reset
	# -no-reboot
	# -no-shutdown
	# -cpu host
	# -enable-kvm
)

set(KERNEL_CC_FLAGS
	-Wall
	-Wextra
	-Wold-style-cast
	-ffreestanding
	-fno-exceptions
	-fno-rtti
	-nostdlib
	-lgcc
	-mno-red-zone
	-mgeneral-regs-only
	-fno-use-cxa-atexit
	-fno-threadsafe-statics
	-Wno-sized-deallocation
	-mcmodel=kernel
	-masm=intel
	-m64
)
# TODO apply -mgeneral-regs-only and -mno-red-zone to relevant files only
# TODO remove -fno-threadsafe-statics when multithreading is implemented
# FIXME -fno-use-cxa-atexit seems to just outright disable global constructors
#		even when we include the crtbegin.o and crtend.o files
#		this was an issue even when they were linked in the correct order
# VERIFY should -Wno-sized-deallocation be removed?

set(KERNEL_ASM_FLAGS
	# TODO add assembly flags here if needed
	-w-all
	# FIXME suppress only "uninitialized space declared in non-BSS section" warnings
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(KERNEL_CC_FLAGS
		${KERNEL_CC_FLAGS}
		-g
		-O0
		-fno-omit-frame-pointer
	)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	set(KERNEL_CC_FLAGS
		${KERNEL_CC_FLAGS}
		-O3
		-fomit-frame-pointer
	)
elseif(CMAKE_BUILT_TYPE MATCHES RelWithDebInfo)
	set(KERNEL_CC_FLAGS
		${KERNEL_CC_FLAGS}
		-g
		-O3
		-fno-omit-frame-pointer
	)
else()
	message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

add_compile_definitions(__is_kernel)
add_compile_definitions(__arch_${ARCH})

add_subdirectory(src/kernel)
add_subdirectory(src/lib)

# execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${KERNEL_CC_FLAGS} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${KERNEL_CC_FLAGS} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "CRTBEGIN_PATH: ${CRTBEGIN_PATH}")
message(STATUS "CRTEND_PATH: ${CRTEND_PATH}")

set(SOURCES
	${CRTI_SRC}
	${KERNEL_SOURCES}
	${LIB_SOURCES}
	${CRTN_SRC}
)
# FIXME see comment below about linking order

set(CMAKE_EXECUTABLE_SUFFIX ".bin")
add_executable(kernel ${SOURCES})
target_compile_options(kernel
	PRIVATE
	$<$<COMPILE_LANGUAGE:C>:${KERNEL_CC_FLAGS}>
	$<$<COMPILE_LANGUAGE:CXX>:${KERNEL_CC_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM_NASM>:${KERNEL_ASM_FLAGS}>)

target_link_options(kernel
	PRIVATE
		-T ${LINKER_SCRIPT}
		-ffreestanding
		-nostdlib
		-lgcc
		-mno-red-zone
		-mcmodel=kernel
		-m64
)

target_link_libraries(kernel
	${CRTBEGIN_PATH}
	${CRTEND_PATH}
)
# FIXME global constructor objects need to be linked in the correct order
#		correct order: crti.o crtbegin.o {kernel} crtend.o crtn.o
#		current order: crti.o {kernel} crtn.o crtbegin.o crtend.o
#		crti.o and crtn.o can be found in kernel/arch/${ARCH}/crt/*
#		crtbegin.o and crtend.o are provided by the compiler

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/kernel.iso
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/iso/boot/grub
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel.bin ${CMAKE_BINARY_DIR}/iso/boot/kernel.bin
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/conf/grub/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub/grub.cfg
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/make-iso.cmake
	DEPENDS kernel
	DEPENDS ${CMAKE_SOURCE_DIR}/conf/grub/grub.cfg
	VERBATIM
)

add_custom_target(iso ALL DEPENDS ${CMAKE_BINARY_DIR}/kernel.iso)

add_custom_target(run
	COMMAND qemu-system-x86_64 -cdrom ${CMAKE_BINARY_DIR}/kernel.iso ${QEMU_FLAGS}
	DEPENDS iso
)

add_custom_target(debug
	COMMAND qemu-system-x86_64 -cdrom ${CMAKE_BINARY_DIR}/kernel.iso ${QEMU_FLAGS} -S -gdb tcp::1234
	DEPENDS iso
)