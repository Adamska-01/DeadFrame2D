#include "Engine/ECS/Component/UI/Slider.h"
#include <algorithm>
#include <string>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	Slider::Slider()
	{
	}


	UIElementType Slider::GetElementType() const
	{
		return UIElementType::RANGE;
	}

	void Slider::OnInteractableCreated()
	{
		// Bounds before value: the backend clamps what it is given against the range it knows about, so
		// a value written first would be measured against the default 0..100 and land somewhere else.
		ApplyOrientation();
		ApplyRange();
		ApplyValue();
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

		if (payload.numericValue == value)
			return;

		value = payload.numericValue;

		OnValueChanged.Broadcast(value);
	}

	void Slider::OnActivationStateChanged(bool isActivated)
	{
		// Activation must focus the slider so arrow-key input can change its value.
		if (isActivated && !IsFocused())
		{
			Focus();
		}
	}


	void Slider::SetRange(float min, float max)
	{
		minimum = min;
		maximum = std::max(min, max);

		ApplyRange();

		SetValue(value);
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

		// Stored before applying: writing the attribute makes the backend raise its own change event,
		// and that pass then sees nothing new. It only speaks up if the backend snapped the value to a
		// step, which is a change the caller does want to hear about.
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
			// Before activation, block the input so reaching the slider does not change its value.
			return IsActivated()
				? UINavigationResponse::CONSUME
				: UINavigationResponse::BLOCK;
		}

		// Once activated, block navigation on the cross axis while adjusting the slider.
		return IsActivated()
			? UINavigationResponse::BLOCK
			: UINavigationResponse::MOVE_FOCUS;
	}


	void Slider::ApplyRange()
	{
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
	}

	void Slider::ApplyValue()
	{
		SetAttribute(UIAttribute::VALUE, std::to_string(value));
	}

	void Slider::ApplyOrientation()
	{
		SetAttribute(UIAttribute::ORIENTATION, vertical ? "vertical" : "horizontal");
	}
}