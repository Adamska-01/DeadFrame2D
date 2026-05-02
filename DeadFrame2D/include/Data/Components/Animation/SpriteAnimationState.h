#pragma once
#include <SDL_render.h>


namespace DF2D::Data
{
	/** @brief Runtime state of an animation. */
	struct SpriteAnimationState
	{
		/** @brief Current frame index (can be fractional for smooth interpolation). */
		float currentFrame = 0.0f;

		/** @brief Indicates whether the animation has started playing. */
		bool started = false;

		/** @brief Optional flip state. */
		SDL_RendererFlip flipState = SDL_RendererFlip::SDL_FLIP_NONE;
	};
}