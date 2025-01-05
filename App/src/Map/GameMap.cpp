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
	for (auto i = 0; i < mapLayers.size(); i++)
	{
		mapLayers[i]->Update(dt);
	}
}

void GameMap::Draw()
{
	for (auto i = 0; i < mapLayers.size(); i++)
	{
		mapLayers[i]->Draw();
	}
}

void GameMap::Clean()
{
	mapLayers.clear();
}

const std::vector<std::shared_ptr<TileLayer>>& GameMap::GetMapLayers()
{
	return mapLayers;
}