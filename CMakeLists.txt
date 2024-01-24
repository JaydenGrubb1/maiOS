set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/toolchain.cmake)

cmake_minimum_required(VERSION 3.16)
project(mai-os)

set(ARCH x86_64)

set(QEMU_FLAGS
	-m 128M
	-serial stdio
	# -d int,cpu_reset
	# -no-reboot
	# -no-shutdown
	# -cpu host
	# -enable-kvm
)

set(CXX_FLAGS
	-Wall
	-Wextra
	-Wold-style-cast
	-m64
)

set(LD_FLAGS
	-m64
)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CXX_FLAGS
		${CXX_FLAGS}
		-g
		-O0
		-fno-omit-frame-pointer
	)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	set(CXX_FLAGS
		${CXX_FLAGS}
		-O3
		-fomit-frame-pointer
	)
	add_compile_definitions(DNDEBUG)
elseif(CMAKE_BUILT_TYPE MATCHES RelWithDebInfo)
	set(CXX_FLAGS
		${CXX_FLAGS}
		-g
		-O3
		-fno-omit-frame-pointer
	)
	add_compile_definitions(DNDEBUG)
else()
	message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

add_subdirectory(kernel)
# add_subdirectory(lib)
# add_subdirectory(drivers)
# add_subdirectory(usr)

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/initrd.img
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/initrd
	# TODO add files to initrd
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/make-initrd.cmake
	# TODO DEPENDS
	VERBATIM
)

add_custom_target(initrd ALL DEPENDS ${CMAKE_BINARY_DIR}/initrd.img)

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/kernel.iso
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/iso/boot/grub
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/kernel/kernel.bin ${CMAKE_BINARY_DIR}/iso/boot/kernel.bin
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/conf/grub/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub/grub.cfg
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/initrd.img ${CMAKE_BINARY_DIR}/iso/boot/initrd.img
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/tools/make-iso.cmake
	DEPENDS kernel
	DEPENDS ${CMAKE_SOURCE_DIR}/conf/grub/grub.cfg
	DEPENDS initrd
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