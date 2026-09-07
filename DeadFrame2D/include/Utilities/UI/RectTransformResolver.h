#pragma once
#include "Data/Components/UI/Layout/LayoutMode.h"
#include "Data/Components/UI/Layout/LayoutSizeSource.h"
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
	 * @param mode: Whether the element positions itself or a parent lays it out.
	 *
	 * @param sizeSource: Who owns the size. Only meaningful when the parent drives layout, where a
	 * LayoutElement may have taken the size and flex properties for itself.
	 */
	DF2D_API std::vector<ResolvedStyleProperty> ResolveRectTransform(
		const Data::RectTransformProperties& properties,
		Data::LayoutMode mode = Data::LayoutMode::SELF_POSITIONED,
		Data::LayoutSizeSource sizeSource = Data::LayoutSizeSource::RECT_TRANSFORM);
}