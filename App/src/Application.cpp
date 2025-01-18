#include "Application.h"
#include "Prefabs/Bobble.h"
#include <Map/GameMap.h>
#include <SubSystems/AudioManager.h>
#include <SubSystems/Input.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>
#include <EventSystem/EventDispatcher.h>
#include <EventSystem/Events/GameObjectEvents/GameObjectDestroyedEvent.h>


Application::Application()
{
	engine = std::make_unique<Engine>();

	bobble = GameObject::Instantiate<Bobble>(Vector2F{ 100, 100 }, BobbleColor::Blue);

	UIManager::LoadFont("Assets/Fonts/consola.ttf", 20);
	UIManager::LoadFont("Assets/Fonts/consola.ttf", 21);

	auto ptr = TextureManager::LoadTexture("Assets/Sprites/Arrow.png");
	auto ptr1 = TextureManager::LoadTexture("Assets/Sprites/Arrow.png");

	gameMap = GameObject::Instantiate<GameMap>("Assets/Maps/SingleplayerMap.tmx", true);
}

std::optional<int> Application::Run()
{
	return engine->Run();
}