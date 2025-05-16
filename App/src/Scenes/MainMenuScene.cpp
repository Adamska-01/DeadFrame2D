#include "Components/MenuManager.h"
#include "Components/PlayerInputMainMenu.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/SinglePlayerScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/Rendering/ImageScroller.h>
#include <Components/Rendering/Sprite.h>
#include <Components/Transform.h>
#include <Components/UI/Button.h>
#include <Components/UI/Layout/VerticalLayoutGroup.h>
#include <Constants/AssetPaths.h>
#include <Management/SceneManager.h>
#include <Math/Vector2.h>
#include <Models/Blueprints/UI/ButtonBlueprintModel.h>
#include <SubSystems/Events/EventManager.h>
#include <SubSystems/Renderer.h>


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

	// Create Background 
	auto backgroundObject = GameObject::Instantiate<GameObject>();

	backgroundObject.lock()->AddComponent<ImageScroller>(AssetPaths::Backgrounds::BACKGROUND_IMAGE_PATH, ScrollDirection::HORIZONTAL, 50.0f);
	backgroundObject.lock()->GetTransform()->Scale(Vector2(0.5f, 0.5f));

	// Create Title
	auto title = GameObject::Instantiate<GameObject>();

	title.lock()->AddComponent<Sprite>(AssetPaths::Sprites::TITLE_IMAGE_PATH);
	title.lock()->GetTransform()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.2f));
	title.lock()->GetTransform()->Scale(Vector2(0.5f, 0.5f));

	auto layoutPosition = Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.65f);


	// Create Settings Menu
	auto settingsMenuObject = GameObject::Instantiate<GameObject>();
	auto settingsMenuBase = settingsMenuObject.lock()->AddComponent<MenuBase>();

	// TODO: Create menu widgets
	//auto fullscreenCheck = CreateButton(...);

	auto settingsMenuLayout = GameObject::Instantiate<GameObject>();
	settingsMenuLayout.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	settingsMenuLayout.lock()->GetComponent<Transform>()->SetWorldPosition(layoutPosition);
	// TODO: Add menu widgets to the layout
	//settingsMenuLayout.lock()->AddChildGameObject(fullscreenCheck);

	settingsMenuObject.lock()->AddChildGameObject(settingsMenuLayout);


	// Create Main Menu
	auto mainMenuObject = GameObject::Instantiate<GameObject>();
	auto mainMenuBase = mainMenuObject.lock()->AddComponent<MenuBase>();

	auto spButton = CreateButton("SinglePlayer", []() { SceneManager::LoadScene<SinglePlayerScene>(); });
	auto mpButton = CreateButton("Multiplayer", []() { /* Multiplayer logic */ });
	//auto settingsButton = CreateButton("Settings", [menuManagerComponent, settingsMenuBase]() { menuManagerComponent->PushMenu(settingsMenuBase); });
	auto exitButton = CreateButton("Exit", []() { EventManager::SendSystemEvent(SDL_EventType::SDL_QUIT); });

	spButton.lock()->GetComponent<Button>()->SetNavigableUpElement(exitButton.lock()->GetComponent<Button>());
	spButton.lock()->GetComponent<Button>()->SetNavigableDownElement(mpButton.lock()->GetComponent<Button>());
	mpButton.lock()->GetComponent<Button>()->SetNavigableUpElement(spButton.lock()->GetComponent<Button>());
	mpButton.lock()->GetComponent<Button>()->SetNavigableDownElement(exitButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableUpElement(mpButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableDownElement(spButton.lock()->GetComponent<Button>());

	auto mainMenuLayout = GameObject::Instantiate<GameObject>();
	mainMenuLayout.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	mainMenuLayout.lock()->GetComponent<Transform>()->SetWorldPosition(layoutPosition);
	mainMenuLayout.lock()->AddChildGameObject(spButton);
	mainMenuLayout.lock()->AddChildGameObject(mpButton);
	//mainMenuLayout.lock()->AddChildGameObject(settingsButton);
	mainMenuLayout.lock()->AddChildGameObject(exitButton);

	mainMenuObject.lock()->AddChildGameObject(mainMenuLayout);


	// Input
	auto inputObject = GameObject::Instantiate<GameObject>();
	auto inputComponent = inputObject.lock()->AddComponent<PlayerInputMainMenu>();

	// Create MenuManager and push the initial menu onto the stack
	auto menuManagerObject = GameObject::Instantiate<GameObject>();
	menuManagerObject.lock()->AddComponent<MenuManager>()->PushMenu(mainMenuBase);
}