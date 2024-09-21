set(LIBC_SOURCES
	src/bits/stack_guard.cpp
	src/assert.cpp
	src/ctype.cpp
	src/errno.cpp
	src/malloc.cpp
	src/stdio.cpp
	src/stdlib.cpp
	src/string.cpp
	src/unistd.cpp
	src/wchar.cpp
)

list(TRANSFORM LIBC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIBC_SOURCES ${LIBC_SOURCES} PARENT_SCOPE)