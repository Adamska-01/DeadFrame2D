#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Data/Components/UI/Layout/UIAnchor.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Text.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string>


namespace DF2D::Engine
{
	/**
	 * @brief A text label: a RectTransform plus a centre-aligned Text, spawned in one call.
	 *
	 * Two constructors: anchor-preset (UI chrome) or normalized-point (labels at arbitrary fractional
	 * positions, e.g. 0.22 -- not expressible by the nine presets).
	 */
	class DF2D_API TextBlueprint : public GameObject
	{
	private:
		ComponentHandle<Text> text;


	public:
		/** @brief Anchored to a preset with a pixel offset -- e.g. a title, a status line. */
		TextBlueprint(
			const std::string& value,
			float fontSize,
			const Core::Color& color,
			Data::UIAnchor anchor,
			const Core::Vector2F& anchoredPosition,
			const Core::Vector2F& size);

		/** @brief Centred on a normalized (0-1) point of the parent, white, sized to fit one line. */
		TextBlueprint(
			const std::string& value,
			float fontSize,
			const Core::Vector2F& normalizedAnchor,
			float width);


		ComponentHandle<Text> GetText() const;
	};
}