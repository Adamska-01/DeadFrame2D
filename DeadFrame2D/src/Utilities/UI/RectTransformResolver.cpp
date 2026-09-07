#include "Utilities/Helpers/UI/StyleValues.h"
#include "Utilities/UI/RectTransformResolver.h"


namespace DF2D::Utilities::RectTransformResolver
{
	using namespace DF2D::Data;


	std::vector<ResolvedStyleProperty> ResolveRectTransform(
		const RectTransformProperties& properties,
		LayoutMode mode,
		LayoutSizeSource sizeSource)
	{
		auto resolved = std::vector<ResolvedStyleProperty>();
		resolved.reserve(12);

		if (mode == LayoutMode::PARENT_DRIVEN)
		{
			// Everything is written, including the values being cleared, because these are inline
			// properties: an element reparented into a layout group still carries whatever it set
			// while it was placing itself, and only an explicit reset takes those back off.
			resolved.push_back({ UIStyleProperty::POSITION, "relative" });
			resolved.push_back({ UIStyleProperty::LEFT, "auto" });
			resolved.push_back({ UIStyleProperty::RIGHT, "auto" });
			resolved.push_back({ UIStyleProperty::TOP, "auto" });
			resolved.push_back({ UIStyleProperty::BOTTOM, "auto" });
			resolved.push_back({ UIStyleProperty::MARGIN_LEFT, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_TOP, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, "0px" });

			// A LayoutElement is the deliberate override, so when there is one it owns the size and the
			// flex properties outright. Writing them here too would leave two components describing one
			// box, with whichever applied last quietly winning.
			if (sizeSource == LayoutSizeSource::LAYOUT_ELEMENT)
				return resolved;

			// Flex shrinks its children to fit by default, which would quietly resize an element to
			// something other than the size it asked for -- and stop a scrolling container ever
			// overflowing, since its content would shrink to the box instead of running past it.
			resolved.push_back({ UIStyleProperty::FLEX_SHRINK, "0" });

			// A stretched axis means "as much as the parent gives me", which in flow is the parent's
			// business to hand out rather than a fraction this element can name.
			resolved.push_back({
				UIStyleProperty::WIDTH,
				properties.StretchesHorizontally() ? "auto" : StyleValues::ToPixels(properties.sizeDelta.x) });

			resolved.push_back({
				UIStyleProperty::HEIGHT,
				properties.StretchesVertically() ? "auto" : StyleValues::ToPixels(properties.sizeDelta.y) });

			return resolved;
		}

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