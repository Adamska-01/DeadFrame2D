#include "Components/GameFlow/GameManager.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/GameMap.h"
#include "Prefabs/Player.h"
#include "Scenes/SinglePlayerScene.h"
#include <Prefabs/BobbleGrid.h>


void SinglePlayerScene::Enter()
{
	GameObject::Instantiate<GameMap>(AssetPaths::Maps::SINGLEPLAYER_MAP_PATH, true);

	GameObject::Instantiate<Player>();

	auto bobbleGridObject = GameObject::Instantiate<GameObject>();
	bobbleGridObject.lock()->AddComponent<BobbleGrid>();

	auto gameManagerObject = GameObject::Instantiate<GameObject>();
	gameManagerObject.lock()->AddComponent<GameManager>();
}