#include "Scenes/GameScene.h"
#include "Scenes/MainMenuScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/Rendering/ImageScroller.h>
#include <Components/Rendering/Sprite.h>
#include <Components/Transform.h>
#include <Components/UI/Layout/VerticalLayoutGroup.h>
#include <Constants/AssetPaths.h>
#include <Management/SceneManager.h>
#include <Models/Blueprints/UI/ButtonBlueprintModel.h>
#include <SubSystems/Events/EventManager.h>
#include <SubSystems/Renderer.h>


void MainMenuScene::CreateBackground(const Vector2I& renderTargetSize)
{
	auto backgroundObject = GameObject::Instantiate<GameObject>();
	
	backgroundObject.lock()->AddComponent<ImageScroller>(AssetPaths::Backgrounds::BACKGROUND_IMAGE_PATH, ScrollDirection::HORIZONTAL, 50.0f);
	backgroundObject.lock()->GetTransform()->Scale(Vector2(0.5f, 0.5f));
}

void MainMenuScene::CreateTitle(const Vector2I& renderTargetSize)
{
	auto title = GameObject::Instantiate<GameObject>();
	
	title.lock()->AddComponent<Sprite>(AssetPaths::Sprites::TITLE_IMAGE_PATH);
	title.lock()->GetTransform()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.2f));
	title.lock()->GetTransform()->Scale(Vector2(0.5f, 0.5f));
}

void MainMenuScene::SetupLayoutGroup(const Vector2I& renderTargetSize)
{
	auto layoutGroup = GameObject::Instantiate<GameObject>();

	layoutGroup.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	layoutGroup.lock()->GetComponent<Transform>()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.65f));

	auto button1 = CreateButton("SinglePlayer", []() { SceneManager::LoadScene<GameScene>(); });
	auto button2 = CreateButton("Multiplayer", []() { /* Multiplayer logic */ });
	//auto button3 = CreateButton("Settings", []() { /* Settings logic */ });
	auto button4 = CreateButton("Exit", []() { EventManager::SendSystemEvent(SDL_EventType::SDL_QUIT); });

	layoutGroup.lock()->AddChildGameObject(button1);
	layoutGroup.lock()->AddChildGameObject(button2);
	//layoutGroup.lock()->AddChildGameObject(buttonextra);
	layoutGroup.lock()->AddChildGameObject(button4);
}

std::weak_ptr<ButtonBlueprint> MainMenuScene::CreateButton(const std::string& text, const std::function<void()>& onPressedHandler)
{
	ButtonBlueprintModel buttonConfiguration = {
		ButtonComponentModel{
			.onPressedHandler = onPressedHandler,
			.idleButtonSource = AssetPaths::Sprites::BUTTON_IDLE_IMAGE_PATH,
			.hoveredButtonSource = AssetPaths::Sprites::BUTTON_PRESSED_IMAGE_PATH,
			.pressedButtonSource = AssetPaths::Sprites::BUTTON_PRESSED_IMAGE_PATH,
			.buttonSize = Vector2F{ 275.0f, 80.0f }
		},
		TextMeshComponentModel{
			.fontSource = AssetPaths::Fonts::THE_BLAST_FONT_PATH,
			.text = text,
			.textColor = SDL_Color(255, 132, 31),
			.fontSize = 100,
			.textObjectInitialScale = Vector2F(0.25f, 0.25f),
			.linesNumber = 1
		}
	};

	return GameObject::Instantiate<ButtonBlueprint>(buttonConfiguration);
}

void MainMenuScene::Enter()
{
	auto renderTargetSize = Renderer::GetResolutionTarget();

	CreateBackground(renderTargetSize);
	CreateTitle(renderTargetSize);
	SetupLayoutGroup(renderTargetSize);
}