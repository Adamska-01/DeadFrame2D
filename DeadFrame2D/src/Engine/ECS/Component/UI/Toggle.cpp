#include "Engine/ECS/Component/UI/Toggle.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	Toggle::Toggle()
	{
	}


	UIElementType Toggle::GetElementType() const
	{
		return UIElementType::CHECKBOX;
	}

	void Toggle::OnElementCreated()
	{
		ApplyState();
	}

	void Toggle::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
		if (eventType != UIEventType::VALUE_CHANGED)
			return;

		auto newState = !payload.value.empty();

		if (newState == isOn)
			return;

		isOn = newState;

		OnValueChanged.Broadcast(isOn);
	}


	void Toggle::SetIsOn(bool value)
	{
		if (value == isOn)
			return;

		isOn = value;

		// Writing the attribute makes the backend raise its own change event, which arrives back through
		// OnInteraction. The state is already stored by then, so that pass sees no change and stops --
		// leaving this call as the one that broadcasts.
		ApplyState();

		OnValueChanged.Broadcast(isOn);
	}

	bool Toggle::IsOn() const
	{
		return isOn;
	}


	void Toggle::ApplyState()
	{
		if (isOn)
		{
			SetAttribute(UIAttribute::CHECKED, "");
		}
		else
		{
			RemoveAttribute(UIAttribute::CHECKED);
		}
	}
}