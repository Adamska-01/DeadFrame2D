#include "Components/MenuManager.h"
#include "Components/PlayerInputMainMenu.h"
#include "Management/SceneManager.h"
#include <cassert>
#include <Components/Transform.h>
#include <GameObject.h>
#include <SubSystems/Input/Input.h>


PlayerInputMainMenu::PlayerInputMainMenu()
{
	transform = nullptr;
	menuManager = nullptr;
}

void PlayerInputMainMenu::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
	menuManager = SceneManager::FindObjectOfType<MenuManager>();

	assert(menuManager != nullptr && "MenuManager not found in scene!");
}

void PlayerInputMainMenu::Update(float deltaTime)
{
	auto currentMenu = menuManager->GetCurrentMenu();

	if (transform == nullptr || currentMenu == nullptr)
		return;

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

void PlayerInputMainMenu::Draw()
{
}