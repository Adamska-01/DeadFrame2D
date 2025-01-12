#pragma once
#include <GameObject.h>
#include <TileEditor/Parsers/Tiled/TiledCompatibleLayer.h>
#include <TileEditor/Parsers/Tiled/TiledMapParser.h>
#include <vector>


class GameMap : GameObject
{
private:
	TiledMapParser gameMapParser;

	std::vector<std::shared_ptr<TiledCompatibleLayer>> mapLayers;


public:
	GameMap(std::string mapSource);

	~GameMap();

	
	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
	
	const std::vector<std::shared_ptr<TiledCompatibleLayer>>& GetMapLayers();
};