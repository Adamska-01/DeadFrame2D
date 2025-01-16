#include "Map/GameMap.h"


GameMap::GameMap(std::string mapSource)
{
	auto parsedMap = gameMapParser.Parse(mapSource);

	tiledMapCompatibleRenderer = std::make_shared<TiledMapCompatibleRenderer>(parsedMap, true);
}

GameMap::~GameMap()
{
	Clean();
}

void GameMap::Update(float dt)
{
	tiledMapCompatibleRenderer->Update(dt);
}

void GameMap::Draw()
{
	tiledMapCompatibleRenderer->Draw();
}

void GameMap::Clean()
{
}