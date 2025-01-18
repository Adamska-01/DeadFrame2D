#pragma once
#include <GameObject.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>
#include <vector>


class GameMap : public GameObject
{
public:
	GameMap(const char* mapSource, bool extendMapToRenderTarget = false);

	virtual ~GameMap() = default;
};