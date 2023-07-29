set(LIB_SOURCES
	libc/ctype.cpp
	libc/stdio.cpp
	libc/stdlib.cpp
	libc/string.cpp
)

list(TRANSFORM LIB_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)