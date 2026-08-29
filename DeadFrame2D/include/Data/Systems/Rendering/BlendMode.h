#pragma once


namespace DF2D::Data
{
	/** @brief How a drawn texture is composited against what is already on the target. */
	enum class BlendMode
	{
		NONE,

		ALPHA,

		ADDITIVE
	};
}