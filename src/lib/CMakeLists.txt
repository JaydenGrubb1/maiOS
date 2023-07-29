set(LIBC_SOURCES
	libc/ctype.cpp
	libc/stdio.cpp
	libc/stdlib.cpp
	libc/string.cpp
)

set(LIBCXX_SOURCES
	# TODO Add libc++ sources
)

set(LIB_SOURCES
	${LIBC_SOURCES}
	${LIBCXX_SOURCES}
)

list(TRANSFORM LIB_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)