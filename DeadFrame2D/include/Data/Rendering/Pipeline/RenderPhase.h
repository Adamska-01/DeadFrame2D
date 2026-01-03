#pragma once
#include <cstdint>


namespace DeadFrame2D::Data
{
	enum class RenderPhase : uint8_t
	{
		WORLD,

		SCREEN_SPACE_CAMERA_UI,

		DEBUG_WORLD,

		SCREEN_SPACE_OVERLAY_UI,

		DEBUG_OVERLAY
	};
}