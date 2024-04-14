set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/libc)
include_directories(${CMAKE_SOURCE_DIR}/include/libc++)

set(CXX_FLAGS
	${CXX_FLAGS}
	-ffreestanding
	-fno-exceptions
	-fno-rtti
	-nostdlib
	-lgcc
	-mno-red-zone
	-fno-use-cxa-atexit
	-fstack-protector-all
	-fno-threadsafe-statics
	-Wno-sized-deallocation
	-mcmodel=kernel
	-masm=intel
)
# TODO remove -fno-threadsafe-statics when multithreading is implemented
# FIXME -fno-use-cxa-atexit seems to just outright disable global constructors
#		even when we include the crtbegin.o and crtend.o files
#		this was an issue even when they were linked in the correct order
# VERIFY should -Wno-sized-deallocation be removed?
# VERIFY what files should -mno-red-zone be added to?

set(ASM_FLAGS
	# TODO add assembly flags here if needed
	-w-all
	# FIXME suppress only "uninitialized space declared in non-BSS section" warnings
)

set(LD_FLAGS
	${LD_FLAGS}
	-ffreestanding
	-nostdlib
	-lgcc
	-mno-red-zone
	-mcmodel=kernel
)

add_compile_definitions(__is_kernel)
add_compile_definitions(__arch_${ARCH})

add_subdirectory(${CMAKE_SOURCE_DIR}/kernel/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/libc ${CMAKE_BINARY_DIR}/kernel/libc)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/libc++ ${CMAKE_BINARY_DIR}/kernel/libc++)

# execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CXX_FLAGS} -print-file-name=crtbegin.o OUTPUT_VARIABLE CRTBEGIN_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${CXX_FLAGS} -print-file-name=crtend.o OUTPUT_VARIABLE CRTEND_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# FIXME add -mcmodel=kernel multilib variants of crtbegin.o and crtend.o

message(STATUS "CRTBEGIN_PATH: ${CRTBEGIN_PATH}")
message(STATUS "CRTEND_PATH: ${CRTEND_PATH}")

set(SOURCES
	${CRTI_SRC}
	${KERNEL_SOURCES}
	${LIBC_SOURCES}
	${LIBCXX_SOURCES}
	${CRTN_SRC}
)
# FIXME see comment below about linking order

set(CMAKE_EXECUTABLE_SUFFIX ".bin")
add_executable(kernel ${SOURCES})

target_compile_options(kernel
	PRIVATE
	$<$<COMPILE_LANGUAGE:C>:${CXX_FLAGS}>
	$<$<COMPILE_LANGUAGE:CXX>:${CXX_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM_NASM>:${ASM_FLAGS}>
)
target_link_options(kernel
	PRIVATE
		-T ${LINKER_SCRIPT}
		${LD_FLAGS}
)

set_target_properties(kernel PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

target_link_libraries(kernel
	${CRTBEGIN_PATH}
	${CRTEND_PATH}
)
# FIXME global constructor objects need to be linked in the correct order
#		correct order: crti.o crtbegin.o {kernel} crtend.o crtn.o
#		current order: crti.o {kernel} crtn.o crtbegin.o crtend.o
#		crti.o and crtn.o can be found in kernel/arch/${ARCH}/crt/*
#		crtbegin.o and crtend.o are provided by the compiler