#pragma once
#include "TiledLayer.h"
#include "TiledTileSet.h"
#include "TiledObjectGroup.h"


struct TiledMap
{
	int width;

	int height;

	int tileSize;

	std::vector<TiledTileSet> tileSets;

	std::vector<TiledLayer> layers;

	std::vector<TiledObjectGroup> objectGroups;
};