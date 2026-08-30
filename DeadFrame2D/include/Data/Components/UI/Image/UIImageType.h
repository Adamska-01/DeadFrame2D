#pragma once


namespace DF2D::Data
{
	/** @brief How an image fills the element it is drawn into. */
	enum class UIImageType
	{
		/** @brief Stretched to the element's box. */
		SIMPLE,

		/** @brief Nine-sliced, so corners keep their size while edges and centre stretch. */
		SLICED,

		/** @brief Repeated at its native size. */
		TILED
	};
}