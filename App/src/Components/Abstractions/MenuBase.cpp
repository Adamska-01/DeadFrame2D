#include "Components/Abstractions/MenuBase.h"
#include "Components/MenuManager.h"
#include <Components/UI/Abstractions/IInteractableUI.h>
#include <GameObject.h>
#include <Management/SceneManager.h>
#include <Tools/Helpers/Guards.h>


MenuBase::MenuBase()
{
	allInteractables.clear();

	menuManager = nullptr;
	previousMenu = nullptr;
	selectedInteractable = nullptr;
}

void MenuBase::Navigate(IInteractableUI* (*getNext)(IInteractableUI*))
{
	if (selectedInteractable == nullptr)
	{
		if (!allInteractables.empty())
		{
			selectedInteractable = allInteractables.front();
			
			selectedInteractable->OnPointerEnter();
		}

		return;
	}

	auto next = getNext(selectedInteractable);

	if (next == nullptr)
		return;

	selectedInteractable->OnPointerExit();
	next->OnPointerEnter();

	selectedInteractable = next;
}

void MenuBase::Init()
{
	menuManager = SceneManager::FindObjectOfType<MenuManager>();
	allInteractables = OwningObject.lock()->GetComponentsInChildren<IInteractableUI>(true);

	Tools::Helpers::GuardAgainstNull(menuManager, "MenuManager not found in scene!");
}

void MenuBase::Start()
{

}

void MenuBase::Update(float deltaTime)
{
}

void MenuBase::Draw()
{
}

void MenuBase::Show()
{
	OwningObject.lock()->SetActive(true);
}

void MenuBase::Hide()
{
	OwningObject.lock()->SetActive(false);
}

void MenuBase::NavigateLeft()
{
	Navigate([](IInteractableUI* current)
		{
			return current->GetLeftInteractable();
		});
}

void MenuBase::NavigateRight()
{
	Navigate([](IInteractableUI* current) 
		{
			return current->GetRightInteractable();
		});
}

void MenuBase::NavigateUp()
{
	Navigate([](IInteractableUI* current)
		{
			return current->GetUpInteractable();
		});
}

void MenuBase::NavigateDown()
{
	Navigate([](IInteractableUI* current)
		{
			return current->GetDownInteractable();
		});
}

void MenuBase::Confirm()
{
	if (selectedInteractable == nullptr)
	{
		if (!allInteractables.empty())
		{
			selectedInteractable = allInteractables.front();

			selectedInteractable->OnPointerEnter();
		}

		return;
	}

	selectedInteractable->OnPointerUp();
}

void MenuBase::GoBack()
{
	if (previousMenu == nullptr)
		return;

	Hide();
	previousMenu->Show();
}

void MenuBase::SetPreviousMenu(MenuBase* previousMenu)
{
	previousMenu = previousMenu;
}