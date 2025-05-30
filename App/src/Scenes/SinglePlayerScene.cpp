#include "Constants/AssetPaths.h"
#include "Prefabs/GameBoard.h"
#include "Scenes/SinglePlayerScene.h"


void SinglePlayerScene::Enter()
{
	GameObject::Instantiate<GameBoard>(AssetPaths::Maps::SINGLEPLAYER_MAP_PATH);
}