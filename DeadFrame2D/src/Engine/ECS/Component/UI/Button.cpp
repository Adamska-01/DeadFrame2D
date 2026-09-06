#include "Core/Context/Systems/UI/Abstractions/IUIBackend.h"
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
		if (auto* backend = Backend())
		{
			backend->SetElementText(element, label);
		}
	}


	void Button::SetText(std::string_view value)
	{
		auto newLabel = std::string(value);

		if (newLabel == label)
			return;

		label = std::move(newLabel);

		// Cached above and written here only once there is an element: scenes set the caption while
		// building, which runs before this component is initialised.
		if (auto* backend = Backend())
		{
			backend->SetElementText(element, label);
		}
	}

	const std::string& Button::GetText() const
	{
		return label;
	}
}