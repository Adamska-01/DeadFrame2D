#pragma once
#include <GameObject.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>
#include <vector>


class GameMap : GameObject
{
public:
	GameMap(const char* mapSource, bool extendMapToRenderTarget = false);

	~GameMap() = default;
};