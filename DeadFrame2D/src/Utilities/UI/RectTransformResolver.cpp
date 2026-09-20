#include "Utilities/Helpers/UI/StyleValues.h"
#include "Utilities/UI/RectTransformResolver.h"


namespace DF2D::Utilities::RectTransformResolver
{
	using namespace DF2D::Data;


	namespace
	{
		/**
		 * @brief The length an axis resolves to, taking content fitting into account.
		 *
		 * RCSS has no fit-content, so "auto" is used instead. It shrinks to the content for both shapes
		 * this resolver produces: a flex item, and an absolutely positioned box with one edge left auto.
		 */
		std::string ResolveLength(float size, bool stretches, SizeFitMode fit)
		{
			return stretches || fit == SizeFitMode::FIT_CONTENT ? "auto" : StyleValues::ToPixels(size);
		}
	}


	std::vector<ResolvedStyleProperty> ResolveRectTransform(
		const RectTransformProperties& properties,
		const LayoutContext& context)
	{
		auto resolved = std::vector<ResolvedStyleProperty>();
		resolved.reserve(12);

		// Scale dp values with the canvas. Anchors are normalized 0-1 values and need no scaling.
		auto sizeDelta = properties.sizeDelta * context.uiScaleFactor;
		auto anchoredPosition = properties.anchoredPosition * context.uiScaleFactor;

		if (context.mode == LayoutMode::PARENT_DRIVEN)
		{
			// The cleared values are written too, not just skipped. These are inline properties, so an
			// element reparented into a layout group still carries what it set while placing itself.
			resolved.push_back({ UIStyleProperty::POSITION, "relative" });
			resolved.push_back({ UIStyleProperty::LEFT, "auto" });
			resolved.push_back({ UIStyleProperty::RIGHT, "auto" });
			resolved.push_back({ UIStyleProperty::TOP, "auto" });
			resolved.push_back({ UIStyleProperty::BOTTOM, "auto" });
			resolved.push_back({ UIStyleProperty::MARGIN_LEFT, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_TOP, "0px" });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, "0px" });

			// A LayoutElement writes the size and flex properties itself. Writing them here as well would
			// mean two writers, with whichever applied last winning.
			if (context.sizeSource == LayoutSizeSource::LAYOUT_ELEMENT)
				return resolved;

			// flex-shrink defaults to 1, which resizes the element away from the size it asked for. It
			// also stops a scrolling container overflowing, leaving nothing to scroll.
			resolved.push_back({ UIStyleProperty::FLEX_SHRINK, "0" });

			// A stretched axis means "fill the parent", which in flow the parent decides, not the child.
			resolved.push_back({
				UIStyleProperty::WIDTH,
				ResolveLength(sizeDelta.x, properties.StretchesHorizontally(), context.horizontalFit) });

			resolved.push_back({
				UIStyleProperty::HEIGHT,
				ResolveLength(sizeDelta.y, properties.StretchesVertically(), context.verticalFit) });

			return resolved;
		}

		resolved.push_back({ UIStyleProperty::POSITION, "absolute" });

		// Horizontal axis.
		if (properties.StretchesHorizontally())
		{
			// Both edges are pinned, so the width falls out of the two insets and must not be set.
			resolved.push_back({ UIStyleProperty::LEFT, StyleValues::ToPercent(properties.anchorMin.x) });
			resolved.push_back({ UIStyleProperty::RIGHT, StyleValues::ToPercent(1.0f - properties.anchorMax.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_LEFT, StyleValues::ToPixels(sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, StyleValues::ToPixels(sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::WIDTH, "auto" });
		}
		else
		{
			// Pinned to a point: place the anchor, then pull back by the pivot so the pivot lands on it.
			resolved.push_back({ UIStyleProperty::LEFT, StyleValues::ToPercent(properties.anchorMin.x) });
			resolved.push_back({ UIStyleProperty::RIGHT, "auto" });
			resolved.push_back({ UIStyleProperty::WIDTH, ResolveLength(sizeDelta.x, false, context.horizontalFit) });
			resolved.push_back({
				UIStyleProperty::MARGIN_LEFT,
				StyleValues::ToPixels(anchoredPosition.x - properties.pivot.x * sizeDelta.x) });
			resolved.push_back({ UIStyleProperty::MARGIN_RIGHT, "0px" });
		}

		// Vertical axis, mirroring the horizontal one.
		if (properties.StretchesVertically())
		{
			resolved.push_back({ UIStyleProperty::TOP, StyleValues::ToPercent(properties.anchorMin.y) });
			resolved.push_back({ UIStyleProperty::BOTTOM, StyleValues::ToPercent(1.0f - properties.anchorMax.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_TOP, StyleValues::ToPixels(sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, StyleValues::ToPixels(sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::HEIGHT, "auto" });
		}
		else
		{
			resolved.push_back({ UIStyleProperty::TOP, StyleValues::ToPercent(properties.anchorMin.y) });
			resolved.push_back({ UIStyleProperty::BOTTOM, "auto" });
			resolved.push_back({ UIStyleProperty::HEIGHT, ResolveLength(sizeDelta.y, false, context.verticalFit) });
			resolved.push_back({
				UIStyleProperty::MARGIN_TOP,
				StyleValues::ToPixels(anchoredPosition.y - properties.pivot.y * sizeDelta.y) });
			resolved.push_back({ UIStyleProperty::MARGIN_BOTTOM, "0px" });
		}

		return resolved;
	}
}