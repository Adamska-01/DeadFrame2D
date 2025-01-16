#pragma once
#include "TiledLayer.h"
#include "TiledTileSet.h"


struct TiledMap
{
	int width;

	int height;

	int tileSize;

	std::vector<TiledTileSet> tileSets;

	std::vector<TiledLayer> layers;
};