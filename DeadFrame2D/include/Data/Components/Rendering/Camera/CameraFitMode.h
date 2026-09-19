#pragma once


namespace DF2D::Data
{
	/**
	 * @brief How a camera's view scales when the render target's resolution differs from its reference.
	 */
	enum class CameraFitMode
	{
		/** @brief No scaling: a bigger render target simply reveals more world, a smaller one less. */
		ENVELOPE,

		/** @brief Scales so the reference width always fits exactly; the height reveals more or less. */
		MATCH_WIDTH,

		/** @brief Scales so the reference height always fits exactly; the width reveals more or less. */
		MATCH_HEIGHT,

		/** @brief Scales by whichever axis would reveal less, so the full reference view always fits. */
		LETTERBOX
	};
}