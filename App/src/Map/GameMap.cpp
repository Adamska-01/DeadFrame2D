#include "Map/GameMap.h"
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>


GameMap::GameMap(const char* mapSource, bool extendMapToRenderTarget)
{
	AddComponent<TiledMapCompatibleRenderer>(mapSource, extendMapToRenderTarget);
}