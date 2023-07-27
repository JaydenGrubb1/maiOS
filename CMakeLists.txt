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

include_directories(include)

set(KERNEL_CC_FLAGS
	-Wall
	-Wextra
	-ffreestanding
	-fno-exceptions
	-fno-rtti
	-nostdlib
	-lgcc
	-mno-red-zone
	-mgeneral-regs-only
	-fno-use-cxa-atexit
	-fno-threadsafe-statics
	-masm=intel
)
# TODO apply -mgeneral-regs-only and -mno-red-zone to relevant files only
# TODO add exception support
# TODO remove -fno-threadsafe-statics when multithreading is implemented
# FIXME -fno-use-cxa-atexit seems to just outright disable global constructors
#		even when we include the crtbegin.o and crtend.o files

set(KERNEL_ASM_FLAGS
	# TODO Add assembly flags here if needed
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(KERNEL_CC_FLAGS ${KERNEL_CC_FLAGS} -g -O0)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	set(KERNEL_CC_FLAGS ${KERNEL_CC_FLAGS} -O3)
else()
	message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

add_subdirectory(src/kernel)
add_subdirectory(src/lib)

add_compile_definitions(__is_kernel)
add_compile_definitions(__arch_x86_64)

# execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

set(SOURCES
	${KERNEL_SOURCES}
	${LIB_SOURCES}
)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/kernel/arch/x86_64/linker.ld)

set(CMAKE_EXECUTABLE_SUFFIX ".bin")
add_executable(kernel ${SOURCES})
target_compile_options(kernel
	PRIVATE
	$<$<COMPILE_LANGUAGE:CXX>:${KERNEL_CC_FLAGS}>
	$<$<COMPILE_LANGUAGE:C>:${KERNEL_CC_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM_NASM>:${KERNEL_ASM_FLAGS}>)

target_link_options(kernel
	PRIVATE
		-T ${LINKER_SCRIPT}
		-ffreestanding
		-nostdlib
		-lgcc
		-mno-red-zone
)

# target_link_libraries(kernel
# 	${CRTBEGIN_PATH}
# 	${CRTEND_PATH}
# )