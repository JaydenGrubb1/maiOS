set(LIBC_SOURCES
	src/assert.cpp
	src/ctype.cpp
	src/stdio.cpp
	src/stdlib.cpp
	src/string.cpp
	src/wchar.cpp
)

list(TRANSFORM LIBC_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIBC_SOURCES ${LIBC_SOURCES} PARENT_SCOPE)