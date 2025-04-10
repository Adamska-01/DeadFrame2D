#include "Application.h"
#include "GameObjects/BobbleGrid.h"
#include "GameObjects/Cannon.h"
#include "Prefabs/Bobble.h"
#include <Map/GameMap.h>


Application::Application()
{
	gameMap = GameObject::Instantiate<GameMap>("App/Assets/Maps/SingleplayerMap.tmx", true);
	bobble = GameObject::Instantiate<Bobble>(Vector2F{ 100, 100 }, BobbleColor::Blue);
	cannon = GameObject::Instantiate<Cannon>();
	bobbleGrid = GameObject::Instantiate<BobbleGrid>();
}