#include "Core/Context/Systems/UI/Elements/UIDropdown.h"
#include "Core/Context/Systems/UI/UIManager.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;


	UIDropdown::UIDropdown(UIManager* manager, UIElementID id)
		: manager(manager),
		id(id)
	{
	}


	bool UIDropdown::IsValid() const
	{
		return manager != nullptr && id != 0;
	}


	int UIDropdown::AddOption(const std::string& text, const std::string& value)
	{
		return IsValid() ? manager->AddDropdownOption(id, text, value) : -1;
	}

	void UIDropdown::ClearOptions()
	{
		if (IsValid())
		{
			manager->ClearDropdownOptions(id);
		}
	}

	void UIDropdown::SetSelection(int index)
	{
		if (IsValid())
		{
			manager->SetDropdownSelection(id, index);
		}
	}

	int UIDropdown::GetSelection() const
	{
		return IsValid() ? manager->GetDropdownSelection(id) : -1;
	}
}