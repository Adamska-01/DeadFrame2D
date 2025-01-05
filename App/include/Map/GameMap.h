#pragma once
#include "GameMapParser.h"
#include "Properties/GameMapProperties.h"
#include <IObject.h>
#include <vector>


class GameMap : IObject
{
private:
	GameMapParser gameMapParser;

	std::vector<std::shared_ptr<TileLayer<GameMapProperties>>> mapLayers;


public:
	GameMap(std::string mapSource);

	~GameMap();

	
	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
	
	const std::vector<std::shared_ptr<TileLayer<GameMapProperties>>>& GetMapLayers();
};