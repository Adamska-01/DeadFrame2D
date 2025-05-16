#include "Components/GameMapParser.h"
#include "Prefabs/GameMap.h"
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>


GameMap::GameMap(std::string_view mapSource, bool extendMapToRenderTarget)
{
	gameMapParser = AddComponent<GameMapParser>(mapSource);

	tileRenderer = AddComponent<TiledMapCompatibleRenderer>(gameMapParser->RetrieveRenderMap(), extendMapToRenderTarget);
	tileCollider = AddComponent<TiledMapCompatibleCollider2D>(gameMapParser->RetrieveCollisionMap());

	auto bodyDef = BodyDefinition2D();
	bodyDef.type = BodyType2D::Static;

	AddComponent<RigidBody2D>(bodyDef);
}