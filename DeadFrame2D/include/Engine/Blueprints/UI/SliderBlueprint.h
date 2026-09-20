#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Slider.h"
#include "Engine/ECS/Component/UI/UINavigator.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief A slider whose fill is two swappable textures (unfilled background, filled foreground
	 * clipped to the value's fraction) instead of a track/handle. The real <input type="range"> still
	 * drives dragging/keyboard input; style its track/progress/handle to nothing so only the two
	 * textures draw.
	 */
	class DF2D_API SliderBlueprint : public GameObject
	{
	private:
		ComponentHandle<Slider> slider;

		ComponentHandle<Image> backgroundImage;

		ComponentHandle<Image> foregroundImage;

		ComponentHandle<RectTransform> clipRect;

		ComponentHandle<UINavigator> navigator;

		Core::Vector2F size;

		int tier = 0;

		bool editing = false;


		void Refresh(float value);

		void EnterEditing();


	public:
		SliderBlueprint(
			const Core::Vector2F& size,
			std::string_view unfilledTexturePath,
			std::string_view filledTexturePath,
			float minValue,
			float maxValue,
			float initialValue,
			ComponentHandle<UINavigator> navigatorHandle = ComponentHandle<UINavigator>());


		/** @brief The underlying range widget, for the owner's own OnValueChanged binding. */
		ComponentHandle<Slider> GetSlider() const;

		bool IsEditing() const;

		/** @brief Exits editing mode. Safe to call whether or not this slider is currently editing. */
		void ExitEditing();
	};
}