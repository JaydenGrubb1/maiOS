/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-01
 * @brief C++ style memory allocation functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <new>

#include <kernel/arch/memory.h>

void *operator new(size_t size) {
	return Memory::allocate(size);
}

void *operator new[](size_t size) {
	return Memory::allocate(size);
}

void *operator new(size_t size, std::align_val_t align) {
	return Memory::allocate(size, static_cast<size_t>(align));
}

void *operator new[](size_t size, std::align_val_t align) {
	return Memory::allocate(size, static_cast<size_t>(align));
}

void operator delete(void *ptr, size_t size) {
	Memory::deallocate(ptr, size);
}

void operator delete[](void *ptr, size_t size) {
	Memory::deallocate(ptr, size);
}

void operator delete(void *ptr, size_t size, std::align_val_t align) {
	Memory::deallocate(ptr, size, static_cast<size_t>(align));
}

void operator delete[](void *ptr, size_t size, std::align_val_t align) {
	Memory::deallocate(ptr, size, static_cast<size_t>(align));
}