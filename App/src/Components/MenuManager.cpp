#include "Components/MenuManager.h"
#include <Management/SceneManager.h>


MenuManager::MenuManager()
{
	menuStack = std::stack<MenuBase*>();
}

void MenuManager::Init()
{
	// Deactivate all menus
	for (auto menu : SceneManager::FindObjectsOfType<MenuBase>())
	{
		menu->Hide();
	}

	if (!menuStack.empty())
	{
		menuStack.top()->Show();
	}
}

void MenuManager::Start()
{

}

void MenuManager::Update(float deltaTime)
{

}

void MenuManager::Draw()
{

}

void MenuManager::PushMenu(MenuBase* menu)
{
	if (menu == nullptr)
		return;

	menu->Show();

	if (!menuStack.empty())
	{
		menuStack.top()->Hide();
	}

	menuStack.push(menu);
}

void MenuManager::PopMenu()
{
	if (menuStack.empty())
		return;

	menuStack.top()->Hide();
	menuStack.pop();
}

MenuBase* MenuManager::GetCurrentMenu() const
{
	return menuStack.top();
}