#pragma once


namespace DF2D::Data
{
	/** @brief Region a particle emitter picks its spawn positions from. */
	enum class EmitterShape
	{
		POINT,

		CIRCLE,

		BOX
	};
}