#pragma once
#include "Data/Systems/Rendering/RenderFlip.h"


namespace DF2D::Data
{
	/** @brief Runtime state of an animation. */
	struct SpriteAnimationState
	{
		/** @brief Current frame index (can be fractional for smooth interpolation). */
		float currentFrame = 0.0f;

		/** @brief Optional flip state. */
		RenderFlip flipState = RenderFlip::NONE;
	};
}