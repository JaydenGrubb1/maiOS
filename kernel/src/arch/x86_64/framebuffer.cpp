/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-25
 * @brief Initialize and provide access to the framebuffer
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdint.h>

#include <cassert>

#include <kernel/arch/x86_64/framebuffer.h>
#include <kernel/arch/x86_64/memory/paging.h>
#include <kernel/arch/x86_64/multiboot2.h>
#include <kernel/debug.h>
#include <kernel/defines.h>

using namespace Graphics;

static uint32_t *_addr = nullptr;
static uint32_t _width = 0;
static uint32_t _height = 0;
static uint32_t _pitch = 0;

void Framebuffer::init(void) {
	Debug::log("Initializing framebuffer...");

	auto fb = static_cast<Multiboot2::FramebufferInfo const *>(Multiboot2::get_entry(Multiboot2::BootInfoType::FRAMEBUFFER_INFO));
	assert(fb != nullptr);

	// TODO fail gracefully if framebuffer is not supported
	assert(fb->color_type == Multiboot2::FramebufferInfo::ColorType::RGB);
	assert(fb->bpp == 32);

	_addr = reinterpret_cast<uint32_t *>(fb->addr);
	_width = fb->width;
	_height = fb->height;
	_pitch = fb->pitch;

	Debug::log_info("Framebuffer info:");
	Debug::log("- Memory: [%p-%p]", _addr, _addr + (_pitch * _height));
	Debug::log("- Resolution: %ux%u", _width, _height);
	Debug::log("- Pitch: %u", _pitch);
	Debug::log("- BPP: %u", fb->bpp);
	Debug::log("- Color type: %u (%s)", static_cast<uint8_t>(fb->color_type), "RGB");

	auto num_pages = Memory::Paging::round_up(_pitch * _height) / Memory::Paging::PAGE_SIZE;
	Debug::log_info("Mapping %zu pages for framebuffer...", num_pages);
	for (size_t i = 0; i < num_pages; i++) {
		auto addr = reinterpret_cast<uintptr_t>(_addr) + (i * Memory::Paging::PAGE_SIZE);
		Memory::Paging::map_page(addr, addr, Memory::Paging::Flags::WRITE_COMBINING | Memory::Paging::Flags::WRITABLE);
	}

	Debug::log_ok("Framebuffer initialized");
}

int Framebuffer::width(void) {
	return _width;
}

int Framebuffer::height(void) {
	return _height;
}

int Framebuffer::pitch(void) {
	return _pitch;
}

uint32_t *Framebuffer::addr(void) {
	return _addr;
}