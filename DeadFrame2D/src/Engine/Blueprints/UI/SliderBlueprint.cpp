#include "Engine/Blueprints/UI/SliderBlueprint.h"
#include "Core/Math/Color.h"
#include "Core/Math/MathUtils.h"
#include "Data/Components/UI/Layout/UIAnchor.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "Engine/ECS/Component/UI/Layout/HorizontalLayoutGroup.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	namespace
	{
		struct TierColors
		{
			Color unfilled;
			Color filled;
		};

		TierColors GetTierColors(bool activated, bool focused)
		{
			if (activated)
				return { Color{ 0x4a, 0x4a, 0x52, 255 }, Color{ 0xff, 0xff, 0xff, 255 } };

			if (focused)
				return { Color{ 0x3a, 0x3a, 0x40, 255 }, Color{ 0xcf, 0xcf, 0xd6, 255 } };

			return { Color{ 0x2a, 0x2a, 0x2e, 255 }, Color{ 0x8a, 0x8a, 0x92, 255 } };
		}
	}


	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		std::string_view unfilledTexturePath,
		std::string_view filledTexturePath,
		float minValue,
		float maxValue,
		float initialValue)
		: size(size)
	{
		AddComponent<RectTransform>()->SetSizeDelta(size);

		// Background: always fully visible underneath everything else.
		auto backgroundObj = GameObject::Instantiate<GameObject>();
		backgroundObj->SetParent(GetObjectHandle());
		backgroundObj->AddComponent<RectTransform>()->SetSizeDelta(size);
		backgroundImage = backgroundObj->AddComponent<Image>();
		backgroundImage->SetSprite(unfilledTexturePath);

		// Foreground: full-size, but inside a container clipped to the value's fraction of the width.
		auto clipObj = GameObject::Instantiate<GameObject>();
		clipObj->SetParent(GetObjectHandle());

		clipRect = clipObj->AddComponent<RectTransform>();
		clipRect->SetAnchorPreset(UIAnchor::TOP_LEFT);
		clipRect->SetAnchoredPosition(Vector2F::Zero);
		clipRect->SetSizeDelta(Vector2F(0.0f, size.y));

		// Set directly, not via a stylesheet class: this widget has no stylesheet to depend on.
		clipRect->SetStyleProperty(UIStyleProperty::OVERFLOW_X, "hidden");
		clipRect->SetStyleProperty(UIStyleProperty::OVERFLOW_Y, "hidden");

		// RmlUi's overflow:hidden never clips an absolutely positioned child, so this forces the
		// foreground into a real flex child (only one, not for arranging anything) that actually can.
		clipObj->AddComponent<HorizontalLayoutGroup>();

		auto foregroundObj = GameObject::Instantiate<GameObject>();
		foregroundObj->SetParent(clipObj);
		foregroundObj->AddComponent<RectTransform>()->SetSizeDelta(size);
		foregroundImage = foregroundObj->AddComponent<Image>();
		foregroundImage->SetSprite(filledTexturePath);

		// Created last so it paints on top of the two texture layers.
		auto sliderObj = GameObject::Instantiate<GameObject>();
		sliderObj->SetParent(GetObjectHandle());
		sliderObj->AddComponent<RectTransform>()->SetSizeDelta(size);

		slider = sliderObj->AddComponent<Slider>();
		slider->SetRange(minValue, maxValue);
		slider->SetValue(initialValue);

		Refresh();

		// The three looks are just the slider's own states drawn differently, so each state change is
		// the same repaint. Which state the slider is in is not this blueprint's business.
		slider->OnValueChanged.AddHandle(slider, [this](float) { Refresh(); });
		slider->OnFocusChanged.AddHandle(slider, [this](bool) { Refresh(); });
		slider->OnActivationChanged.AddHandle(slider, [this](bool) { Refresh(); });
	}

	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		float minValue,
		float maxValue,
		float initialValue)
		: SliderBlueprint(size, "", "", minValue, maxValue, initialValue)
	{
	}

	void SliderBlueprint::Refresh()
	{
		auto range = slider->GetMaximum() - slider->GetMinimum();
		auto value = slider->GetValue();
		auto fraction = range > 0.0f ? MathUtils::Clamp((value - slider->GetMinimum()) / range, 0.0f, 1.0f) : 0.0f;
		auto colors = GetTierColors(slider->IsActivated(), slider->IsFocused());

		clipRect->SetSizeDelta(Vector2F(size.x * fraction, size.y));
		backgroundImage->SetColor(colors.unfilled);
		foregroundImage->SetColor(colors.filled);
	}

	ComponentHandle<Slider> SliderBlueprint::GetSlider() const
	{
		return slider;
	}
}