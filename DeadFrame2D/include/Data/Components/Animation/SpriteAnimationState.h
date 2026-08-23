#pragma once
#include "Data/Systems/Rendering/RenderFlip.h"


namespace DF2D::Data
{
	/** @brief Runtime state of an animation. */
	struct SpriteAnimationState
	{
		/** @brief Current frame index (can be fractional for smooth interpolation). */
		float currentFrame = 0.0f;

		/** @brief Set once a non-looping animation has played through its last frame. */
		bool finished = false;

		/** @brief Optional flip state. */
		RenderFlip flipState = RenderFlip::NONE;
	};
}