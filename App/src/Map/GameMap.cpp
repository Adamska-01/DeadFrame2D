#include "Components/GameMapParser.h"
#include "Map/GameMap.h"
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>


GameMap::GameMap(const char* mapSource, bool extendMapToRenderTarget)
{
	gameMapParser = AddComponent<GameMapParser>(mapSource);

	tileRenderer = AddComponent<TiledMapCompatibleRenderer>(gameMapParser->RetrieveRenderMap(), extendMapToRenderTarget);
	tileCollider = AddComponent<TiledMapCompatibleCollider2D>(gameMapParser->RetrieveCollisionMap());
}