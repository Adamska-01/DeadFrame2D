#pragma once
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
	 */
	DF2D_API std::vector<ResolvedStyleProperty> ResolveRectTransform(const Data::RectTransformProperties& properties);
}