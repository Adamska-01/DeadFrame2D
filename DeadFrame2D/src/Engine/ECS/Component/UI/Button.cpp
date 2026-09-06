#include "Engine/ECS/Component/UI/Button.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	Button::Button()
	{
	}


	UIElementType Button::GetElementType() const
	{
		return UIElementType::BUTTON;
	}

	void Button::OnElementCreated()
	{
		SetElementText(label);
	}


	void Button::SetText(std::string_view value)
	{
		auto newLabel = std::string(value);

		if (newLabel == label)
			return;

		label = std::move(newLabel);

		// Cached above and written here only once there is an element: scenes set the caption while
		// building, which runs before this component is initialised.
		SetElementText(label);
	}

	const std::string& Button::GetText() const
	{
		return label;
	}
}