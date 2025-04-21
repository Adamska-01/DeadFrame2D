#include "Constants/AssetPaths.h"
#include "Prefabs/GameMap.h"
#include "Prefabs/LevelManagerObject.h"
#include "Prefabs/Player.h"
#include "Scenes/GameScene.h"


void GameScene::Enter()
{
	gameMap = GameObject::Instantiate<GameMap>(AssetPaths::SINGLEPLAYER_MAP_PATH, true);
	player = GameObject::Instantiate<Player>(PlayerIdentifier::PLAYER_1, Vector2F(2.0f, 2.0f));
	levelManager = GameObject::Instantiate<LevelManagerObject>(GameMode::SINGLE_PLAYER);
}