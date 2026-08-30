#include "Utilities/Helpers/UI/StyleValues.h"
#include "Utilities/UI/RectTransformResolver.h"


namespace DF2D::Utilities::RectTransformResolver
{
	using namespace DF2D::Data;


	std::vector<ResolvedStyleProperty> ResolveRectTransform(const RectTransformProperties& properties)
	{
		auto resolved = std::vector<ResolvedStyleProperty>();
		resolved.reserve(10);

		resolved.push_back({ UIStyleProperty::POSITION, "absolute" });

		// Horizontal axis.
		if (properties.StretchesHorizontally())
		{
			// Both edges are pinned, so the width falls out of the two insets and must not be set.
			resolved.push_back({ UIStyleProperty::LEFT, StyleValues::ToPercent(properties.anchorMin.x) });
			resolved.push_back({ UIStyleProperty::RIGHT, StyleValues::ToPercent(1.0f - properties.anchorMax.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_LEFT, StyleValues::ToPixels(properties.sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, StyleValues::ToPixels(properties.sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::WIDTH, "auto" });
		}
		else
		{
			// Pinned to a point: place the anchor, then pull back by the pivot so the pivot lands on it.
			resolved.push_back({ UIStyleProperty::LEFT, StyleValues::ToPercent(properties.anchorMin.x) });
			resolved.push_back({ UIStyleProperty::RIGHT, "auto" });
			resolved.push_back({ UIStyleProperty::WIDTH, StyleValues::ToPixels(properties.sizeDelta.x) });
			resolved.push_back({
				UIStyleProperty::MARGIN_LEFT,
				StyleValues::ToPixels(properties.anchoredPosition.x - properties.pivot.x * properties.sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, "0px" });
		}

		// Vertical axis, mirroring the horizontal one.
		if (properties.StretchesVertically())
		{
			resolved.push_back({ UIStyleProperty::TOP, StyleValues::ToPercent(properties.anchorMin.y) });
			resolved.push_back({ UIStyleProperty::BOTTOM, StyleValues::ToPercent(1.0f - properties.anchorMax.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_TOP, StyleValues::ToPixels(properties.sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, StyleValues::ToPixels(properties.sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::HEIGHT, "auto" });
		}
		else
		{
			resolved.push_back({ UIStyleProperty::TOP, StyleValues::ToPercent(properties.anchorMin.y) });
			resolved.push_back({ UIStyleProperty::BOTTOM, "auto" });
			resolved.push_back({ UIStyleProperty::HEIGHT, StyleValues::ToPixels(properties.sizeDelta.y) });
			resolved.push_back({
				UIStyleProperty::MARGIN_TOP,
				StyleValues::ToPixels(properties.anchoredPosition.y - properties.pivot.y * properties.sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, "0px" });
		}

		return resolved;
	}
}