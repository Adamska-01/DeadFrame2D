#pragma once
#include "Data/Components/UI/Layout/LayoutContext.h"
#include "Data/Components/UI/RectTransform/RectTransformProperties.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DF2D::Utilities::RectTransformResolver
{
	/** @brief One resolved style property and the value to set it to. */
	struct ResolvedStyleProperty
	{
		Data::UIStyleProperty property;

		std::string value;
	};


	/**
	 * @brief Turns the anchor model into the style properties that express it.
	 *
	 * @param context: What the surrounding tree implies -- who places the element, who owns its size,
	 * and whether either axis is to shrink to its content.
	 */
	DF2D_API std::vector<ResolvedStyleProperty> ResolveRectTransform(
		const Data::RectTransformProperties& properties,
		const Data::LayoutContext& context = Data::LayoutContext{});
}