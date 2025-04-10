#include "Application.h"
#include "Prefabs/Bobble.h"
#include "GameObjects/BobbleGrid.h"
#include "Components/Collisions/CircleCollider2D.h"
#include <Map/GameMap.h>
#include <SubSystems/TextureManager.h>
#include <SubSystems/UIManager.h>


Application::Application()
{
	gameMap = GameObject::Instantiate<GameMap>("App/Assets/Maps/SingleplayerMap.tmx", true);
	bobble = GameObject::Instantiate<Bobble>(Vector2F{ 100, 100 }, BobbleColor::Blue);
	bobbleGrid = GameObject::Instantiate<BobbleGrid>();

	//UIManager::LoadFont("Assets/Fonts/consola.ttf", 20);

	//auto ptr = TextureManager::LoadTexture("Assets/Sprites/Arrow.png");

	gameMap = GameObject::Instantiate<GameMap>("Assets/Maps/SingleplayerMap.tmx", true);
}