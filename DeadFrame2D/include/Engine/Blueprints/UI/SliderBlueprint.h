#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/UI/Slider/SliderColors.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Slider.h"
#include "Engine/ECS/Component/UI/Text.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string>
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief Builds a layered slider with an unfilled background and a filled foreground.
	 *
	 * The widget is composed of:
	 *
	 *     slider
	 *     +- background
	 *     +- fill area
	 *        +- fill
	 *     +- handle
	 *     +- value
	 *
	 * Textures can be provided for the background and fill, or omitted to use flat colour fills.
	 * SliderBlueprint only builds the hierarchy; Slider controls the value, layout and colours.
	 *
	 * Handle and value label are optional and can be hidden without affecting slider behaviour.
	 */
	class DF2D_API SliderBlueprint : public GameObject
	{
	private:
		ComponentHandle<Slider> slider;

		ObjectHandle<GameObject> handleObject;

		ObjectHandle<GameObject> valueObject;

		ComponentHandle<Text> valueText;

		std::string valueSuffix;


		/** @brief Updates the value label with the current slider value and suffix. */
		void RefreshValueText();


	public:
		SliderBlueprint(
			const Core::Vector2F& size,
			std::string_view unfilledTexturePath,
			std::string_view filledTexturePath,
			float minValue,
			float maxValue,
			float initialValue);

		/** @brief Creates the same layered slider using flat colour fills instead of textures. */
		SliderBlueprint(
			const Core::Vector2F& size,
			float minValue,
			float maxValue,
			float initialValue);


		/** @brief Returns the underlying slider for value changes and other configuration. */
		ComponentHandle<Slider> GetSlider() const;

		/** @brief Shows or hides the value handle. */
		void SetHandleVisible(bool value);

		/** @brief Shows or hides the value label. */
		void SetValueLabelVisible(bool value);

		/** @brief Appends a suffix to the displayed value, such as "%" or "px". */
		void SetValueSuffix(std::string_view value);

		/** @brief Sets the colours used by the slider's visual layers. */
		void SetColors(const Data::SliderColors& value);
	};
}