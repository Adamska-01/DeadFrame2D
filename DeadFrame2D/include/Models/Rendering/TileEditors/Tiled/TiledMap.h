#pragma once
#include "Models/Rendering/TileEditors/Tiled/TiledLayer.h"
#include "Models/Rendering/TileEditors/Tiled/TiledObjectGroup.h"
#include "Models/Rendering/TileEditors/Tiled/TiledTileSet.h"


namespace DeadFrame2D::Models
{
	struct TiledMap
	{
		int width;

		int height;

		int tileSize;

		std::vector<TiledTileSet> tileSets;

		std::vector<TiledLayer> layers;

		std::vector<TiledObjectGroup> objectGroups;
	};
}