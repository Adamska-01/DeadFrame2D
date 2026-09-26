#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Slider.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief A slider rendered as separate unfilled and filled layers.
	 *
	 * The layers can use textures or flat colours. The underlying Slider handles input and state.
	 */
	class DF2D_API SliderBlueprint : public GameObject
	{
	private:
		ComponentHandle<Slider> slider;

		ComponentHandle<Image> backgroundImage;

		ComponentHandle<Image> foregroundImage;

		ComponentHandle<RectTransform> clipRect;

		Core::Vector2F size;


		void Refresh();


	public:
		SliderBlueprint(
			const Core::Vector2F& size,
			std::string_view unfilledTexturePath,
			std::string_view filledTexturePath,
			float minValue,
			float maxValue,
			float initialValue);

		/** @brief Same widget with no textures -- background/foreground are flat colour fills instead. */
		SliderBlueprint(
			const Core::Vector2F& size,
			float minValue,
			float maxValue,
			float initialValue);


		/** @brief The underlying range widget, for the owner's own OnValueChanged binding. */
		ComponentHandle<Slider> GetSlider() const;
	};
}