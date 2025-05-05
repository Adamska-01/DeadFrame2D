#include "Components/Abstractions/MenuBase.h"
#include "Components/MenuManager.h"
#include <cassert>
#include <Components/UI/Abstractions/IInteractableUI.h>
#include <GameObject.h>
#include <Management/SceneManager.h>


MenuBase::MenuBase()
{
	allInteractables.clear();

	menuManager = nullptr;
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
	allInteractables = SceneManager::FindObjectsOfType<IInteractableUI>();

	assert(menuManager != nullptr && "MenuManager not found in scene!");
}

void MenuBase::Update(float deltaTime)
{
}

void MenuBase::Draw()
{
}

void MenuBase::Show()
{
	OwningObject->SetActive(true);
}

void MenuBase::Hide()
{
	OwningObject->SetActive(false);
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
	menuManager->PopMenu();
}