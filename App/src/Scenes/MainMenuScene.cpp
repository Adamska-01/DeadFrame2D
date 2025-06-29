#include "Components/MenuManager.h"
#include "Prefabs/AudioClipObject.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/SinglePlayerScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/Audio/AudioListener.h>
#include <Components/Rendering/ImageScroller.h>
#include <Components/Rendering/Sprite.h>
#include <Components/Transform.h>
#include <Components/UI/Button.h>
#include <Components/UI/Canvas.h>
#include <Components/UI/Layout/VerticalLayoutGroup.h>
#include <Constants/AssetPaths.h>
#include <Management/SceneManager.h>
#include <SubSystems/Events/EventManager.h>
#include <SubSystems/Renderer.h>


void MainMenuScene::Enter()
{
	auto canvasObject = GameObject::Instantiate<GameObject>();
	canvasObject.lock()->AddComponent<Canvas>();

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

	canvasObject.lock()->AddChildGameObject(settingsMenuObject);

	// TODO: Create menu widgets
	//auto fullscreenCheck = CreateButton(...);

	auto settingsMenuLayout = GameObject::Instantiate<GameObject>();
	
	
	settingsMenuLayout.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	settingsMenuLayout.lock()->GetComponent<Transform>()->SetWorldPosition(layoutPosition);
	// TODO: Add menu widgets to the layout
	//settingsMenuLayout.lock()->AddChildGameObject(fullscreenCheck);

	// Create Main Menu
	auto mainMenuObject = GameObject::Instantiate<GameObject>();
	auto mainMenuBase = mainMenuObject.lock()->AddComponent<MenuBase>();

	canvasObject.lock()->AddChildGameObject(mainMenuObject);

	auto enterCallback = MakeAudioPlayAndDestroyCallback(AssetPaths::Audio::BOBBLE_POP, Vector2F::Zero, 0.5f, false, false, 1.0f);

	auto spButton = CreateButton("Play Game", []() { SceneManager::LoadScene<SinglePlayerScene>(); }, enterCallback);
	//auto mpButton = CreateButton("Multiplayer", []() { /* Multiplayer logic */ }, enterCallback);
	//auto settingsButton = CreateButton("Settings", [menuManagerComponent, settingsMenuBase]() { menuManagerComponent->PushMenu(settingsMenuBase); });
	auto exitButton = CreateButton("Exit", []() { EventManager::SendSystemEvent(SDL_EventType::SDL_QUIT); }, enterCallback);


	spButton.lock()->GetComponent<Button>()->SetNavigableUpElement(exitButton.lock()->GetComponent<Button>());
	spButton.lock()->GetComponent<Button>()->SetNavigableDownElement(exitButton.lock()->GetComponent<Button>());
	//mpButton.lock()->GetComponent<Button>()->SetNavigableUpElement(spButton.lock()->GetComponent<Button>());
	//mpButton.lock()->GetComponent<Button>()->SetNavigableDownElement(exitButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableUpElement(spButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableDownElement(spButton.lock()->GetComponent<Button>());

	auto mainMenuLayout = GameObject::Instantiate<GameObject>();

	mainMenuLayout.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	mainMenuLayout.lock()->GetComponent<Transform>()->SetWorldPosition(layoutPosition);
	mainMenuLayout.lock()->AddChildGameObject(spButton);
	//mainMenuLayout.lock()->AddChildGameObject(mpButton);
	//mainMenuLayout.lock()->AddChildGameObject(settingsButton);
	mainMenuLayout.lock()->AddChildGameObject(exitButton);
	mainMenuObject.lock()->AddChildGameObject(mainMenuLayout);

	// Create MenuManager and push the initial menu onto the stack
	auto menuManagerObject = GameObject::Instantiate<GameObject>();
	auto menuManagerComponent = menuManagerObject.lock()->AddComponent<MenuManager>();
	
	menuManagerComponent->RegisterMenu(MenuID::MAIN_MENU, mainMenuBase);
	menuManagerComponent->ShowMenu(MenuID::MAIN_MENU);

	// Sound
	auto soundSourceObj = GameObject::Instantiate<AudioClipObject>(
		AssetPaths::Audio::MENU_MUSIC,
		Vector2F::Zero,
		0.3f,
		true,
		true);


	auto soundListenerObj = GameObject::Instantiate<GameObject>();
	auto listener = soundListenerObj.lock()->AddComponent<AudioListener>();

	//Window::SetWindowMode(WindowMode::BORDERLESS);
	//Window::SetResolution(Vector2I(2560, 1440));
}