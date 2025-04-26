#include "Scenes/GameScene.h"
#include "Scenes/MainMenuScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/Transform.h>
#include <Constants/AssetPaths.h>
#include <Constants/SharedResourcePaths.h>
#include <Management/SceneManager.h>
#include <Models/Blueprints/UI/ButtonBlueprintModel.h>
#include <SubSystems/Window.h>


void MainMenuScene::Enter()
{
	auto buttonConfiguration = ButtonBlueprintModel
	{
		ButtonComponentModel
		{
			.onPressedHandler = []() { SceneManager::LoadScene<GameScene>(); },
			.idleButtonSource = AssetPaths::Sprites::BUTTON_IDLE_IMAGE_PATH,
			.hoveredButtonSource = AssetPaths::Sprites::BUTTON_HOVERED_IMAGE_PATH,
			.pressedButtonSource = AssetPaths::Sprites::BUTTON_PRESSED_IMAGE_PATH,
			.buttonSize = Vector2F{ 300.0f, 100.0f }
		},
		TextMeshComponentModel
		{
			.fontSource = SharedResourcePaths::Fonts::CONSOLAS_FONT_PATH,
			.text = "Single Player",
			.linesNumber = 1
		}
	};

	auto button = GameObject::Instantiate<ButtonBlueprint>(buttonConfiguration);

	button.lock()->GetTransform()->SetWorldPosition(Vector2F(Window::GetResolution().x * 0.5f, Window::GetResolution().y * 0.5f));
}