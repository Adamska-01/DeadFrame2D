#pragma once
#include "Core/Math/Vector2.h"
#include <string>


namespace DF2D::Data
{
	/**
	 * @brief The values carried by a UI element event.
	 *
	 * One struct rather than a variant: the fields are small, the set is fixed, and every consumer
	 * already knows which of them its own UIEventType populates.
	 */
	struct UIEventPayload
	{
		/** @brief Current value as text, for VALUE_CHANGED and SUBMIT. */
		std::string value;

		/** @brief Current value as a number, for range-like elements. */
		float numericValue = 0.0f;

		/** @brief Current selection or on/off state, for toggles and dropdowns. */
		int index = 0;

		/** @brief Two-dimensional value, for SCROLL. */
		Core::Vector2F offset;
	};
}