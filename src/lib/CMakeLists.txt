set(LIB_SOURCES
	ctype.cpp
	stdio.cpp
	stdlib.cpp
	string.cpp
)

list(TRANSFORM LIB_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(LIB_SOURCES ${LIB_SOURCES} PARENT_SCOPE)