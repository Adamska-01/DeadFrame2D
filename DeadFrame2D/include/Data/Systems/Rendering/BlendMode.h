#pragma once


namespace DF2D::Data
{
	/** @brief How a drawn texture is composited against what is already on the target. */
	enum class BlendMode
	{
		NONE,

		ALPHA,

		/**
		 * @brief Alpha blending for sources whose color channels are already multiplied by alpha.
		 *
		 * Needed by geometry producers that emit premultiplied vertex colors, such as the UI backend.
		 * Compositing those with plain ALPHA multiplies by alpha a second time and darkens edges.
		 */
		PREMULTIPLIED_ALPHA,

		ADDITIVE
	};
}