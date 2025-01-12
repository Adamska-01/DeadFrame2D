#pragma once
#include "TileEditor/Parsers/Tiled/Models/TiledSetModel.h"
#include "TileEditor/Parsers/Tiled/Models/TiledMapWallProperties.h"
#include "TileEditor/TileLayer.h"



class TiledCompatibleLayer : public TileLayer<int, TiledSetModel, TiledMapWallProperties>
{
public:
	TiledCompatibleLayer(int tileSize, int rowCount, int colCount, std::vector<std::vector<int>> tileMap, std::vector<TiledSetModel> tileSets, std::optional<TiledMapWallProperties> properties);


	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};