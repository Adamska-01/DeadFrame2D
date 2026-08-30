#pragma once
#include "Core/Math/Vector2.h"


namespace DF2D::Data
{
	/**
	 * @brief The anchor model describing where a UI element sits inside its parent.
	 *
	 * anchorMin and anchorMax are normalized positions in the parent, 0 being its left/top edge and 1
	 * its right/bottom. When the two are equal on an axis the element is pinned to a point and sized
	 * by sizeDelta; when they differ it stretches between them and sizeDelta becomes an inset from
	 * each anchor. Both cases resolve to a plain absolutely-positioned box, so the layout engine, not
	 * the engine's own code, works out the final rectangle.
	 */
	struct RectTransformProperties
	{
		Core::Vector2F anchorMin = Core::Vector2F(0.5f, 0.5f);

		Core::Vector2F anchorMax = Core::Vector2F(0.5f, 0.5f);

		/** @brief The point within the element that anchoredPosition places, in normalized coords. */
		Core::Vector2F pivot = Core::Vector2F(0.5f, 0.5f);

		/** @brief Offset of the pivot from the anchor, in pixels. Only used for point anchors. */
		Core::Vector2F anchoredPosition = Core::Vector2F::Zero;

		/**
		 * @brief Size in pixels on a pinned axis, or the inset from both anchors on a stretched one.
		 */
		Core::Vector2F sizeDelta = Core::Vector2F(100.0f, 100.0f);


		/** @brief Whether the element stretches horizontally rather than being pinned to a point. */
		inline bool StretchesHorizontally() const
		{
			return anchorMin.x != anchorMax.x;
		}

		/** @brief Whether the element stretches vertically rather than being pinned to a point. */
		inline bool StretchesVertically() const
		{
			return anchorMin.y != anchorMax.y;
		}
	};
}