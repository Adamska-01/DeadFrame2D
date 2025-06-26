#include "Components/MenuManager.h"
#include <SubSystems/Input/Input.h>


MenuManager::MenuManager()
	: allMenus(), 
	activeMenus()
{
}

void MenuManager::Init()
{
}

void MenuManager::Start()
{

}

void MenuManager::Update(float deltaTime)
{
	for (auto currentMenu : activeMenus)
	{
		if (Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Up")
			|| Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Up2"))
		{
			currentMenu->NavigateUp();
		}
		if (Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Down")
			|| Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Down2"))
		{
			currentMenu->NavigateDown();
		}
		if (Input::IsButtonPressed(PlayerInputSlot::PLAYER_1, "Jump"))
		{
			currentMenu->Confirm();
		}
	}
}

void MenuManager::Draw()
{

}

void MenuManager::ShowMenu(MenuID menuID)
{
	auto it = allMenus.find(menuID);

	if (it == allMenus.end())
		return;

	auto menu = it->second;

	menu->Show();
	activeMenus.push_back(menu);
}

void MenuManager::HideMenu(MenuID menuID)
{
	auto it = allMenus.find(menuID);

	if (it == allMenus.end())
		return;

	auto menu = it->second;

	menu->Hide();

	activeMenus.erase(
		std::remove(
			activeMenus.begin(), 
			activeMenus.end(), 
			menu),
		activeMenus.end());
}

void MenuManager::HideAll()
{
	for (auto* menu : activeMenus) 
	{
		menu->Hide();
	}

	activeMenus.clear();
}

void MenuManager::RegisterMenu(MenuID menuID, MenuBase* menu)
{
	allMenus[menuID] = menu;
}

MenuBase* MenuManager::GetMenu(MenuID menuID)
{
	auto it = allMenus.find(menuID);

	return it != allMenus.end() ? it->second : nullptr;
}

const std::vector<MenuBase*>& MenuManager::GetActiveMenus()
{
	return activeMenus;
}