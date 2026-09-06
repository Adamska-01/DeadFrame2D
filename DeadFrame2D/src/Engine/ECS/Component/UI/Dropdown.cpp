#include "Core/Context/Systems/UI/Context/UIContext.h"
#include "Engine/ECS/Component/UI/Dropdown.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;

	namespace
	{
		const std::string EmptyOption;
	}


	Dropdown::Dropdown()
		: selectedIndex(-1)
	{
	}


	UIElementType Dropdown::GetElementType() const
	{
		return UIElementType::DROPDOWN;
	}

	void Dropdown::OnElementCreated()
	{
		ApplyOptions();
		ApplySelection();
	}

	void Dropdown::OnInteraction(UIEventType eventType, const UIEventPayload& payload)
	{
		if (eventType != UIEventType::VALUE_CHANGED)
			return;

		// Asked for rather than read out of the payload: the event reports the option's value, and the
		// index is what this component's API is written in.
		auto newIndex = element.AsDropdown().GetSelection();

		if (newIndex == selectedIndex)
			return;

		selectedIndex = newIndex;

		OnValueChanged.Broadcast(selectedIndex);
	}


	void Dropdown::SetOptions(std::vector<std::string> values)
	{
		options = std::move(values);

		ApplyOptions();

		SetSelectedIndex(options.empty() ? -1 : 0);
	}

	int Dropdown::AddOption(std::string_view value)
	{
		options.push_back(std::string(value));

		auto index = static_cast<int>(options.size()) - 1;

		element.AsDropdown().AddOption(options.back(), options.back());

		return index;
	}

	void Dropdown::ClearOptions()
	{
		options.clear();

		element.AsDropdown().ClearOptions();

		SetSelectedIndex(-1);
	}

	void Dropdown::SetSelectedIndex(int index)
	{
		auto clamped = index >= 0 && index < static_cast<int>(options.size()) ? index : -1;

		if (clamped == selectedIndex)
			return;

		selectedIndex = clamped;

		// Stored before applying, so the change the backend echoes back sees the selection it already
		// holds and stops there, leaving this call as the only broadcast.
		ApplySelection();

		OnValueChanged.Broadcast(selectedIndex);
	}


	int Dropdown::GetSelectedIndex() const
	{
		return selectedIndex;
	}

	const std::string& Dropdown::GetSelectedOption() const
	{
		return selectedIndex >= 0 && selectedIndex < static_cast<int>(options.size())
			? options[selectedIndex]
			: EmptyOption;
	}

	const std::vector<std::string>& Dropdown::GetOptions() const
	{
		return options;
	}


	void Dropdown::ApplyOptions()
	{
		auto dropdown = element.AsDropdown();

		dropdown.ClearOptions();

		for (const auto& option : options)
		{
			dropdown.AddOption(option, option);
		}
	}

	void Dropdown::ApplySelection()
	{
		element.AsDropdown().SetSelection(selectedIndex);
	}
}