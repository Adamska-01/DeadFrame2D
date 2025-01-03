#pragma once
#include "IObject.h"
#include "TileEditor/TileSet.h"
#include <vector>


using TileSetList = std::vector<TileSet>;

using TileMap = std::vector<std::vector<int>>;


class TileLayer : public IObject
{
private:
	TileMap tileMap;
	
	TileSetList tileSets;
	
	int tileSize;
	
	int rowCount;

	int columnCount;


public:
	TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TileSetList tileSets);


	virtual void Update(float dt) override;
	
	virtual void Draw() override;

	virtual void Clean() override;

	TileMap GetTileMap();
};