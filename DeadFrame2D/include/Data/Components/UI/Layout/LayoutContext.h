#pragma once
#include "Data/Components/UI/Layout/LayoutMode.h"
#include "Data/Components/UI/Layout/LayoutSizeSource.h"
#include "Data/Components/UI/Layout/SizeFitMode.h"


namespace DF2D::Data
{
	/**
	 * @brief Layout inputs that come from an element's surroundings rather than from the element itself.
	 *
	 * Passed as one struct so the resolver does not need a growing list of parameters.
	 */
	struct LayoutContext
	{
		LayoutMode mode = LayoutMode::SELF_POSITIONED;

		LayoutSizeSource sizeSource = LayoutSizeSource::RECT_TRANSFORM;

		SizeFitMode horizontalFit = SizeFitMode::UNCONSTRAINED;

		SizeFitMode verticalFit = SizeFitMode::UNCONSTRAINED;

		/** 
		* @brief The canvas's dp-to-pixel multiplier (Canvas::GetUIScaleFactor). 1 leaves dp as pixels
		*/
		float uiScaleFactor = 1.0f;
	};
}