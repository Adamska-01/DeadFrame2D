#include "Data/Systems/UI/UIStyleProperty.h"
#include "Engine/Blueprints/UI/SliderBlueprint.h"
#include "Engine/ECS/Component/UI/Layout/HorizontalLayoutGroup.h"
#include <cmath>
#include <string>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	namespace
	{
		/** @brief Width of the value handle. */
		constexpr auto HandleWidth = 5.0f;

		/** @brief How far past the end of the bar the number sits. */
		constexpr auto ValueLabelGap = 12.0f;

		constexpr auto ValueLabelWidth = 56.0f;

		constexpr auto ValueLabelFontSize = 15.0f;


		/** @brief Prevents the element from intercepting pointer input. */
		void MakeNonInteractive(const ComponentHandle<RectTransform>& rect)
		{
			rect->SetStyleProperty(UIStyleProperty::POINTER_EVENTS, "none");
		}
	}


	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		std::string_view unfilledTexturePath,
		std::string_view filledTexturePath,
		float minValue,
		float maxValue,
		float initialValue)
	{
		AddComponent<RectTransform>()->SetSizeDelta(size);

		// The Slider handles input and layout; the child elements provide its visuals.
		slider = AddComponent<Slider>();

		// Hide the backend's built-in visuals because this slider draws its own layers.
		slider->AddClass("layered-slider");

		slider->SetRange(minValue, maxValue);
		slider->SetValue(initialValue);

		// Background: the whole bar, always fully visible underneath everything else.
		auto backgroundObj = GameObject::Instantiate<GameObject>();
		backgroundObj->SetParent(GetObjectHandle());

		auto backgroundRect = backgroundObj->AddComponent<RectTransform>();
		backgroundRect->SetAnchorMin(Vector2F::Zero);
		backgroundRect->SetAnchorMax(Vector2F::One);
		backgroundRect->SetSizeDelta(Vector2F::Zero);

		MakeNonInteractive(backgroundRect);

		auto backgroundImage = backgroundObj->AddComponent<Image>();
		backgroundImage->SetSprite(unfilledTexturePath);

		auto fillAreaObj = GameObject::Instantiate<GameObject>();
		fillAreaObj->SetParent(GetObjectHandle());

		auto fillAreaRect = fillAreaObj->AddComponent<RectTransform>();
		fillAreaRect->SetAnchorMin(Vector2F::Zero);
		fillAreaRect->SetAnchorMax(Vector2F(0.0f, 1.0f));
		fillAreaRect->SetSizeDelta(Vector2F::Zero);

		MakeNonInteractive(fillAreaRect);

		// Apply clipping directly because this blueprint does not rely on a game stylesheet.
		fillAreaRect->SetStyleProperty(UIStyleProperty::OVERFLOW_X, "hidden");
		fillAreaRect->SetStyleProperty(UIStyleProperty::OVERFLOW_Y, "hidden");

		// The fill must be a layout child for the clipping container to affect it.
		fillAreaObj->AddComponent<HorizontalLayoutGroup>();

		auto fillObj = GameObject::Instantiate<GameObject>();
		fillObj->SetParent(fillAreaObj);
		fillObj->AddComponent<RectTransform>()->SetSizeDelta(size);

		MakeNonInteractive(fillObj->GetComponent<RectTransform>());

		auto fillImage = fillObj->AddComponent<Image>();
		fillImage->SetSprite(filledTexturePath);

		// Keep the handle outside the clipped fill area so it remains visible at the edges.
		handleObject = GameObject::Instantiate<GameObject>();
		handleObject->SetParent(GetObjectHandle());

		auto handleRect = handleObject->AddComponent<RectTransform>();
		handleRect->SetSizeDelta(Vector2F(HandleWidth, size.y));

		MakeNonInteractive(handleRect);

		auto handleImage = handleObject->AddComponent<Image>();

		valueObject = GameObject::Instantiate<GameObject>();
		valueObject->SetParent(GetObjectHandle());

		auto valueRect = valueObject->AddComponent<RectTransform>();
		valueRect->SetAnchorMin(Vector2F(1.0f, 0.5f));
		valueRect->SetAnchorMax(Vector2F(1.0f, 0.5f));
		valueRect->SetPivot(Vector2F(0.0f, 0.5f));
		valueRect->SetAnchoredPosition(Vector2F(ValueLabelGap, 0.0f));
		valueRect->SetSizeDelta(Vector2F(ValueLabelWidth, ValueLabelFontSize * 1.4f));
		valueRect->AddClass("slider-value");

		MakeNonInteractive(valueRect);

		valueText = valueObject->AddComponent<Text>();
		valueText->SetFontSize(ValueLabelFontSize);
		valueText->SetAlignment(TextAlignment::LEFT);
		valueText->SetWordWrap(false);

		// Slider owns the visual state and updates these elements as the value changes.
		slider->SetFillRect(fillAreaRect);
		slider->SetHandleRect(handleRect);
		slider->SetBackgroundImage(backgroundImage);
		slider->SetFillImage(fillImage);
		slider->SetHandleImage(handleImage);

		RefreshValueText();

		slider->OnValueChanged.AddHandle(slider, [this](float) { RefreshValueText(); });
	}

	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		float minValue,
		float maxValue,
		float initialValue)
		: SliderBlueprint(size, "", "", minValue, maxValue, initialValue)
	{
	}

	void SliderBlueprint::RefreshValueText()
	{
		if (valueText == nullptr)
			return;

		valueText->SetText(std::to_string(static_cast<int>(std::lround(slider->GetValue()))) + valueSuffix);
	}

	ComponentHandle<Slider> SliderBlueprint::GetSlider() const
	{
		return slider;
	}

	void SliderBlueprint::SetHandleVisible(bool value)
	{
		if (handleObject != nullptr)
		{
			handleObject->SetActive(value);
		}
	}

	void SliderBlueprint::SetValueLabelVisible(bool value)
	{
		if (valueObject != nullptr)
		{
			valueObject->SetActive(value);
		}
	}

	void SliderBlueprint::SetValueSuffix(std::string_view value)
	{
		valueSuffix = std::string(value);

		RefreshValueText();
	}

	void SliderBlueprint::SetColors(const SliderColors& value)
	{
		slider->SetColors(value);
	}
}