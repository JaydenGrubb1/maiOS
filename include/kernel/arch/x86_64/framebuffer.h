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

#pragma once

namespace Graphics::Framebuffer {
	/**
	 * @brief Initialize the framebuffer
	 *
	 */
	void init(void);

	/**
	 * @brief Get the width of the framebuffer
	 *
	 * @return The width of the framebuffer
	 */
	int width(void);

	/**
	 * @brief Get the height of the framebuffer
	 *
	 * @return The height of the framebuffer
	 */
	int height(void);

	/**
	 * @brief Get the pitch of the framebuffer
	 *
	 * @return The pitch of the framebuffer
	 */
	int pitch(void);

	/**
	 * @brief Get the framebuffer buffer
	 *
	 * @return The framebuffer buffer
	 */
	uint32_t *addr(void);
}