#include "Engine/ECS/Component/UI/InputField.h"
#include <string>


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	InputField::InputField()
	{
	}


	void InputField::ApplyText()
	{
		SetAttribute(UIAttribute::VALUE, text);
	}

	void InputField::ApplyPlaceholder()
	{
		SetAttribute(UIAttribute::PLACEHOLDER, placeholder);
	}

	void InputField::ApplyMaxLength()
	{
		if (maxLength >= 0)
		{
			SetAttribute(UIAttribute::MAX_LENGTH, std::to_string(maxLength));
		}
		else
		{
			RemoveAttribute(UIAttribute::MAX_LENGTH);
		}
	}


	UIElementType InputField::GetElementType() const
	{
		return UIElementType::TEXT_INPUT;
	}

	void InputField::OnElementCreated()
	{
		ApplyMaxLength();
		ApplyPlaceholder();
		ApplyText();
	}

	void InputField::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
		switch (eventType)
		{
		case UIEventType::VALUE_CHANGED:
			if (payload.value == text)
				return;

			text = payload.value;

			OnValueChanged.Broadcast(text);
			break;

		case UIEventType::SUBMIT:
			OnSubmit.Broadcast(text);
			break;

		default:
			break;
		}
	}


	void InputField::SetText(std::string_view value)
	{
		auto newText = std::string(value);

		if (newText == text)
			return;

		text = std::move(newText);

		// Stored before applying, so the change the backend echoes back finds nothing new to report and
		// this call stays the only broadcast.
		ApplyText();

		OnValueChanged.Broadcast(text);
	}

	void InputField::SetPlaceholder(std::string_view value)
	{
		auto newPlaceholder = std::string(value);

		if (newPlaceholder == placeholder)
			return;

		placeholder = std::move(newPlaceholder);

		ApplyPlaceholder();
	}

	void InputField::SetMaxLength(int length)
	{
		if (length == maxLength)
			return;

		maxLength = length;

		ApplyMaxLength();
	}

	const std::string& InputField::GetText() const
	{
		return text;
	}
}