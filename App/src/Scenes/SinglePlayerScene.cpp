#include "Components/GameFlow/GameManager.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/GameBoard.h"
#include "Scenes/SinglePlayerScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/Abstractions/MenuBase.h>
#include <Components/Audio/AudioListener.h>
#include <Components/MenuManager.h>
#include <Components/Rendering/ImageScroller.h>
#include <Components/Transform.h>
#include <Components/UI/Button.h>
#include <Components/UI/Canvas.h>
#include <Components/UI/Image.h>
#include <Components/UI/Layout/VerticalLayoutGroup.h>
#include <Components/UI/LevelTextController.h>
#include <Components/UI/TextMesh.h>
#include <Management/SceneManager.h>
#include <Models/Components/UI/TextMeshComponentModel.h>
#include <Prefabs/AudioClipObject.h>
#include <Prefabs/GameMap.h>
#include <Scenes/MainMenuScene.h>
#include <SubSystems/Events/EventManager.h>
#include <SubSystems/Renderer.h>


MenuBase* SinglePlayerScene::CreateEndScreen(std::string menuTitle, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	auto renderTargetSize = Renderer::GetResolutionTarget();

	auto endScreenMenuObject = GameObject::Instantiate<GameObject>();
	auto endScreenMenuBase = endScreenMenuObject.lock()->AddComponent<MenuBase>();

	auto background = GameObject::Instantiate<GameObject>();
	auto imageBackground = background.lock()->AddComponent<Image>();
	imageBackground->SetWidgetSize(Vector2F(renderTargetSize.x, renderTargetSize.y));
	imageBackground->SetAnchor(UIAnchor::TOP_LEFT);
	imageBackground->SetColor(r, g, b, a);

	auto enterCallback = MakeAudioPlayAndDestroyCallback(AssetPaths::Audio::BOBBLE_POP, Vector2F::Zero, 0.5f, false, false, 1.0f);
	auto title = CreateText(menuTitle);
	auto spButton = CreateButton("Back To Menu", []() { SceneManager::LoadScene<MainMenuScene>(); }, enterCallback);
	auto exitButton = CreateButton("Exit", []() { EventManager::SendSystemEvent(SDL_EventType::SDL_QUIT); }, enterCallback);

	spButton.lock()->GetComponent<Button>()->SetNavigableUpElement(exitButton.lock()->GetComponent<Button>());
	spButton.lock()->GetComponent<Button>()->SetNavigableDownElement(exitButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableUpElement(spButton.lock()->GetComponent<Button>());
	exitButton.lock()->GetComponent<Button>()->SetNavigableDownElement(spButton.lock()->GetComponent<Button>());

	auto menuLayout = GameObject::Instantiate<GameObject>();
	menuLayout.lock()->AddComponent<VerticalLayoutGroup>(20.0f, LayoutPadding());
	menuLayout.lock()->GetComponent<Transform>()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.5f));
	menuLayout.lock()->AddChildGameObject(title);
	menuLayout.lock()->AddChildGameObject(spButton);
	menuLayout.lock()->AddChildGameObject(exitButton);

	endScreenMenuObject.lock()->AddChildGameObject(background);
	endScreenMenuObject.lock()->AddChildGameObject(menuLayout);

	endScreenMenuBase->Hide();

	return endScreenMenuBase;
}

MenuBase* SinglePlayerScene::CreateHUD()
{
	auto renderTargetSize = Renderer::GetResolutionTarget();

	auto hudMenuObject = GameObject::Instantiate<GameObject>();
	auto hudMenuBase = hudMenuObject.lock()->AddComponent<MenuBase>();

	auto title = CreateText("Score: 0");

	title.lock()->GetComponent<TextMesh>()->SetAnchor(UIAnchor::TOP_LEFT);
	
	hudMenuObject.lock()->AddChildGameObject(title);

	return hudMenuBase;
}

void SinglePlayerScene::Enter()
{
	auto backgroundObject = GameObject::Instantiate<GameObject>();
	backgroundObject.lock()->AddComponent<ImageScroller>(AssetPaths::Backgrounds::BACKGROUND_IMAGE_PATH, ScrollDirection::VERTICAL, 50.0f);
	backgroundObject.lock()->GetTransform()->Scale(Vector2(0.5f, 0.5f));

	GameObject::Instantiate<GameMap>(AssetPaths::Maps::SINGLEPLAYER_MAP_PATH);
	auto gameBoard = GameObject::Instantiate<GameBoard>();

	auto renderTargetSize = Renderer::GetResolutionTarget();

	// Create UI
	auto canvasObject = GameObject::Instantiate<GameObject>();
	canvasObject.lock()->AddComponent<Canvas>();

	auto textObj = GameObject::Instantiate<GameObject>();
	textObj.lock()->GetTransform()->SetWorldPosition(Vector2F(renderTargetSize.x * 0.5f, renderTargetSize.y * 0.12f));

	textObj.lock()->AddComponent<TextMesh>(TextMeshComponentModel());
	textObj.lock()->AddComponent<LevelTextController>();


	auto hud = CreateHUD();
	auto gameOverMenuBase = CreateEndScreen("GameOver!", 209, 55, 38, 80);
	auto victoryMenuBase = CreateEndScreen("All Levels Complete!", 9, 173, 53, 80);

	canvasObject.lock()->AddChildGameObject(victoryMenuBase->GetGameObject());
	canvasObject.lock()->AddChildGameObject(gameOverMenuBase->GetGameObject());


	auto menuManagerObject = GameObject::Instantiate<GameObject>();
	auto menuManagerComponent = menuManagerObject.lock()->AddComponent<MenuManager>();

	menuManagerComponent->RegisterMenu(MenuID::HUD, hud);
	menuManagerComponent->RegisterMenu(MenuID::GAME_OVER_MENU, gameOverMenuBase);
	menuManagerComponent->RegisterMenu(MenuID::VICTORY_MENU, victoryMenuBase);

	menuManagerComponent->ShowMenu(MenuID::HUD);

	// Sound
	auto soundSourceObj = GameObject::Instantiate<AudioClipObject>(
		AssetPaths::Audio::GAME_MUSIC,
		Vector2F::Zero,
		0.2f, 
		false,
		true);

	auto soundListenerObj = GameObject::Instantiate<GameObject>();
	auto listener = soundListenerObj.lock()->AddComponent<AudioListener>();


	gameBoard.lock()->GetGameManager()->SetGameMusicSource(soundSourceObj.lock()->GetAudioSource());
	gameBoard.lock()->GetGameManager()->SetScoreTextMesh(hud->GetGameObject().lock()->GetComponentInChildren<TextMesh>());
}