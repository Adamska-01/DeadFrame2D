#include "Core/Math/MathUtils.h"
#include "Engine/ECS/Component/UI/Slider.h"
#include <algorithm>
#include <cmath>
#include <string>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	namespace
	{
		/** @brief Controls how quickly the visuals approach their target state. */
		constexpr auto EasingRate = 14.0f;

		/** @brief Stops easing once the remaining difference is visually negligible. */
		constexpr auto EasingEpsilon = 0.0005f;

		/** @brief Returns a framerate-independent interpolation factor for this frame. */
		float EasingStep(float deltaTime)
		{
			return 1.0f - std::exp(-EasingRate * deltaTime);
		}

		uint8_t LerpChannel(uint8_t from, uint8_t to, float t)
		{
			return static_cast<uint8_t>(MathUtils::Lerp(static_cast<float>(from), static_cast<float>(to), t));
		}

		Color LerpColor(const Color& from, const Color& to, float t)
		{
			return Color{
				LerpChannel(from.r, to.r, t),
				LerpChannel(from.g, to.g, t),
				LerpChannel(from.b, to.b, t),
				LerpChannel(from.a, to.a, t) };
		}

		SliderStateColors LerpColors(const SliderStateColors& from, const SliderStateColors& to, float t)
		{
			return SliderStateColors{
				LerpColor(from.background, to.background, t),
				LerpColor(from.fill, to.fill, t),
				LerpColor(from.handle, to.handle, t) };
		}

		bool IsSameColor(const Color& a, const Color& b)
		{
			return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
		}

		bool IsSameColors(const SliderStateColors& a, const SliderStateColors& b)
		{
			return IsSameColor(a.background, b.background)
				&& IsSameColor(a.fill, b.fill)
				&& IsSameColor(a.handle, b.handle);
		}
	}


	Slider::Slider()
	{
		displayedColors = colors.resting;
	}


	void Slider::ApplyRange()
	{
		// Ignore backend change events while updating the range.
		applyingState = true;

		SetAttribute(UIAttribute::MIN, std::to_string(minimum));
		SetAttribute(UIAttribute::MAX, std::to_string(maximum));

		if (step > 0.0f)
		{
			SetAttribute(UIAttribute::STEP, std::to_string(step));
		}
		else
		{
			RemoveAttribute(UIAttribute::STEP);
		}

		applyingState = false;
	}

	void Slider::ApplyValue()
	{
		SetAttribute(UIAttribute::VALUE, std::to_string(value));
	}

	void Slider::ApplyOrientation()
	{
		SetAttribute(UIAttribute::ORIENTATION, vertical ? "vertical" : "horizontal");
	}


	float Slider::ValueFraction() const
	{
		auto range = maximum - minimum;

		return range > 0.0f ? MathUtils::Clamp((value - minimum) / range, 0.0f, 1.0f) : 0.0f;
	}

	const SliderStateColors& Slider::TargetColors() const
	{
		if (IsActivated())
			return colors.activated;

		return IsFocused() ? colors.focused : colors.resting;
	}

	void Slider::ApplyVisuals()
	{
		// Use anchors so the fill scales with the slider instead of relying on pixel sizes.
		if (fillRect != nullptr)
		{
			auto along = vertical
				? Vector2F(1.0f, displayedFraction)
				: Vector2F(displayedFraction, 1.0f);

			fillRect->SetAnchorMin(Vector2F::Zero);
			fillRect->SetAnchorMax(along);
		}

		// Match the handle pivot to its anchor so its centre stays within the slider bounds.
		if (handleRect != nullptr)
		{
			auto along = vertical
				? Vector2F(0.5f, displayedFraction)
				: Vector2F(displayedFraction, 0.5f);

			handleRect->SetAnchorMin(along);
			handleRect->SetAnchorMax(along);
			handleRect->SetPivot(along);
		}

		if (backgroundGraphic != nullptr)
		{
			backgroundGraphic->SetColor(displayedColors.background);
		}

		if (fillGraphic != nullptr)
		{
			fillGraphic->SetColor(displayedColors.fill);
		}

		if (handleGraphic != nullptr)
		{
			handleGraphic->SetColor(displayedColors.handle);
		}
	}

	void Slider::SnapVisuals()
	{
		displayedFraction = ValueFraction();
		displayedColors = TargetColors();

		ApplyVisuals();
	}


	UIElementType Slider::GetElementType() const
	{
		return UIElementType::RANGE;
	}

	void Slider::OnInteractableCreated()
	{
		ApplyOrientation();

		// Apply bounds first because the backend clamps values against its current range.
		ApplyRange();
		ApplyValue();

		// Initial state should be displayed immediately, without easing.
		SnapVisuals();
	}

	void Slider::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
		// A pointer click should immediately activate the slider.
		if (eventType == UIEventType::CLICK)
		{
			Activate();

			return;
		}

		if (eventType != UIEventType::VALUE_CHANGED)
			return;

		/*
			The backend can emit VALUE_CHANGED while ApplyRange is updating the bounds.
			Those events reflect intermediate backend state, not the slider's actual value.
		*/
		if (applyingState)
			return;

		if (payload.numericValue == value)
			return;

		value = payload.numericValue;

		OnValueChanged.Broadcast(value);
	}

	void Slider::OnActivationStateChanged(bool isActivated)
	{
		// Activation focuses the slider so directional input can adjust its value.
		if (isActivated && !IsFocused())
		{
			Focus();
		}
	}


	void Slider::Update(float deltaTime)
	{
		auto targetFraction = ValueFraction();
		const auto& targetColors = TargetColors();

		// Nothing to ease towards, and nothing written
		if (std::abs(targetFraction - displayedFraction) < EasingEpsilon 
			&& IsSameColors(displayedColors, targetColors))
			return;

		auto step = EasingStep(deltaTime);

		displayedFraction = MathUtils::Lerp(displayedFraction, targetFraction, step);
		displayedColors = LerpColors(displayedColors, targetColors, step);

		ApplyVisuals();
	}


	bool Slider::HasActivationState() const
	{
		return true;
	}

	UINavigationResponse Slider::ResolveNavigation(UINavigationDirection direction) const
	{
		auto isHorizontal = direction == UINavigationDirection::LEFT || direction == UINavigationDirection::RIGHT;
		auto isOwnAxis = isHorizontal != vertical;

		if (isOwnAxis)
		{
			// Before activation, the slider can be focused without changing its value.
			return IsActivated()
				? UINavigationResponse::CONSUME
				: UINavigationResponse::BLOCK;
		}

		// While active, prevent cross-axis navigation from leaving the slider.
		return IsActivated()
			? UINavigationResponse::BLOCK
			: UINavigationResponse::MOVE_FOCUS;
	}


	void Slider::SetRange(float min, float max)
	{
		minimum = min;
		maximum = std::max(min, max);

		ApplyRange();

		// Clamp locally so the value change is reported once, independently of backend events.
		auto desired = std::clamp(value, minimum, maximum);

		if (desired != value)
		{
			value = desired;

			OnValueChanged.Broadcast(value);
		}

		ApplyValue();
	}

	void Slider::SetStep(float newStep)
	{
		step = std::max(0.0f, newStep);

		ApplyRange();
	}

	void Slider::SetValue(float newValue)
	{
		auto clamped = std::clamp(newValue, minimum, maximum);

		if (clamped == value)
			return;

		value = clamped;

		/*
			Write the local value first so the backend's VALUE_CHANGED event is treated as an echo.
			If the backend snaps to a step, that event updates the stored value before the delegate fires.
		*/
		ApplyValue();

		OnValueChanged.Broadcast(value);
	}

	void Slider::SetVertical(bool isVertical)
	{
		if (isVertical == vertical)
			return;

		vertical = isVertical;

		ApplyOrientation();
	}

	void Slider::SetFillRect(const ComponentHandle<RectTransform>& value)
	{
		fillRect = value;

		SnapVisuals();
	}

	void Slider::SetHandleRect(const ComponentHandle<RectTransform>& value)
	{
		handleRect = value;

		SnapVisuals();
	}

	void Slider::SetBackgroundImage(const ComponentHandle<Image>& value)
	{
		backgroundGraphic = value;

		SnapVisuals();
	}

	void Slider::SetFillImage(const ComponentHandle<Image>& value)
	{
		fillGraphic = value;

		SnapVisuals();
	}

	void Slider::SetHandleImage(const ComponentHandle<Image>& value)
	{
		handleGraphic = value;

		SnapVisuals();
	}

	void Slider::SetColors(const SliderColors& value)
	{
		colors = value;

		SnapVisuals();
	}

	const SliderColors& Slider::GetColors() const
	{
		return colors;
	}

	float Slider::GetValue() const
	{
		return value;
	}

	float Slider::GetMinimum() const
	{
		return minimum;
	}

	float Slider::GetMaximum() const
	{
		return maximum;
	}

	bool Slider::IsVertical() const
	{
		return vertical;
	}
}