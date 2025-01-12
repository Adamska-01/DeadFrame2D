#include "Map/GameMap.h"


GameMap::GameMap(std::string mapSource)
{
	mapLayers = gameMapParser.Parse(mapSource);
}

GameMap::~GameMap()
{
	Clean();
}

void GameMap::Update(float dt)
{
	for (const auto& layer : mapLayers)
	{
		layer->Update(dt);
	}
}

void GameMap::Draw()
{
	for (const auto& layer : mapLayers)
	{
		layer->Draw();
	}
}

void GameMap::Clean()
{
	mapLayers.clear();
}

const std::vector<std::shared_ptr<TiledCompatibleLayer>>& GameMap::GetMapLayers()
{
	return mapLayers;
}