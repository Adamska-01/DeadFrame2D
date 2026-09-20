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

		TierColors GetTierColors(int tier)
		{
			switch (tier)
			{
			case 2:  return { Color{ 0x4a, 0x4a, 0x52, 255 }, Color{ 0xff, 0xff, 0xff, 255 } }; // editing
			case 1:  return { Color{ 0x3a, 0x3a, 0x40, 255 }, Color{ 0xcf, 0xcf, 0xd6, 255 } }; // focused
			default: return { Color{ 0x2a, 0x2a, 0x2e, 255 }, Color{ 0x8a, 0x8a, 0x92, 255 } }; // resting
			}
		}
	}


	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		std::string_view unfilledTexturePath,
		std::string_view filledTexturePath,
		float minValue,
		float maxValue,
		float initialValue,
		ComponentHandle<UINavigator> navigatorHandle)
		: navigator(navigatorHandle), size(size)
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

		Refresh(initialValue);

		slider->OnValueChanged.AddHandle(slider, [this](float value) { Refresh(value); });

		// Focus alone only reaches tier 1 (LEFT/RIGHT blocked); Submit/click enters tier 2 editing.
		slider->OnFocusChanged.AddHandle(slider, [this](bool focused)
		{
			if (focused)
			{
				tier = 1;

				if (navigator)
				{
					navigator->SetHorizontalNavigationSuppressed(true);
				}
			}
			else
			{
				tier = 0;

				ExitEditing();

				// ExitEditing alone leaves LEFT/RIGHT blocked (correct while still focused); losing
				// focus entirely needs both axes freed.
				if (navigator)
				{
					navigator->SetHorizontalNavigationSuppressed(false);
				}
			}

			Refresh(slider->GetValue());
		});

		slider->OnClick.AddHandle(slider, [this]()
		{
			if (editing)
				ExitEditing();
			else
				EnterEditing();
		});
	}

	SliderBlueprint::SliderBlueprint(
		const Vector2F& size,
		float minValue,
		float maxValue,
		float initialValue,
		ComponentHandle<UINavigator> navigatorHandle)
		: SliderBlueprint(size, "", "", minValue, maxValue, initialValue, navigatorHandle)
	{
	}

	void SliderBlueprint::Refresh(float value)
	{
		auto range = slider->GetMaximum() - slider->GetMinimum();
		auto fraction = range > 0.0f ? MathUtils::Clamp((value - slider->GetMinimum()) / range, 0.0f, 1.0f) : 0.0f;
		auto colors = GetTierColors(tier);

		clipRect->SetSizeDelta(Vector2F(size.x * fraction, size.y));
		backgroundImage->SetColor(colors.unfilled);
		foregroundImage->SetColor(colors.filled);
	}

	void SliderBlueprint::EnterEditing()
	{
		if (editing)
			return;

		editing = true;
		tier = 2;

		// Editing hands LEFT/RIGHT to the slider; UP/DOWN parked to avoid walking the value by accident.
		if (navigator)
		{
			navigator->SetVerticalNavigationSuppressed(true);
			navigator->SetHorizontalNavigationSuppressed(false);
		}

		Refresh(slider->GetValue());
	}

	ComponentHandle<Slider> SliderBlueprint::GetSlider() const
	{
		return slider;
	}

	bool SliderBlueprint::IsEditing() const
	{
		return editing;
	}

	void SliderBlueprint::ExitEditing()
	{
		if (!editing)
			return;

		editing = false;
		tier = slider->IsFocused() ? 1 : 0;

		if (navigator)
		{
			navigator->SetVerticalNavigationSuppressed(false);
			navigator->SetHorizontalNavigationSuppressed(true);
		}

		Refresh(slider->GetValue());
	}
}