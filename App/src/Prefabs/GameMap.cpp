#include "Data/Physics/BodyDefinition2D.h"
#include "Prefabs/GameMap.h"
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/GameMapParser.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>
#include <Constants/GameMapLayerNames.h>


GameMap::GameMap(std::string_view mapSource)
	: mapSource(mapSource)
{
}

void GameMap::ConstructGameObject()
{
	auto bodyDef = BodyDefinition2D();
	bodyDef.type = BodyType2D::Static;

	// Visual render map
	auto mapObject = GameObject::Instantiate<GameObject>();

	auto toRenderLayerObject = GameObject::Instantiate<GameObject>();

	mapObject.lock()->AddChildGameObject(toRenderLayerObject);

	auto gameMapParser = AddComponent<GameMapParser>(mapSource);

	toRenderLayerObject.lock()->AddComponent<TiledMapCompatibleRenderer>(gameMapParser->RetrieveRenderMap(
		std::vector<std::string_view>(
			Constants::GameMapLayerNames::RENDER_LAYER_NAMES.begin(),
			Constants::GameMapLayerNames::RENDER_LAYER_NAMES.end())),
		true);


	// Side walls render map
	auto sideWallsLayerObject = GameObject::Instantiate<GameObject>();

	mapObject.lock()->AddChildGameObject(sideWallsLayerObject);

	sideWallsLayerObject.lock()->AddComponent<TiledMapCompatibleRenderer>(gameMapParser->RetrieveRenderMap(
		std::vector<std::string_view>(
			Constants::GameMapLayerNames::SIDE_WALLS_LAYER_NAME.begin(),
			Constants::GameMapLayerNames::SIDE_WALLS_LAYER_NAME.end())),
		true);

	sideWallsLayerObject.lock()->AddComponent<TiledMapCompatibleCollider2D>();

	sideWallsLayerObject.lock()->AddComponent<RigidBody2D>(bodyDef);


	// Top walls render map
	auto topWallsLayerObject = GameObject::Instantiate<GameObject>();

	mapObject.lock()->AddChildGameObject(topWallsLayerObject);

	topWallsLayerObject.lock()->AddComponent<TiledMapCompatibleRenderer>(gameMapParser->RetrieveRenderMap(
		std::vector<std::string_view>(
			Constants::GameMapLayerNames::TOP_WALLS_LAYER_NAME.begin(),
			Constants::GameMapLayerNames::TOP_WALLS_LAYER_NAME.end())),
		true);

	auto topWallCollider = topWallsLayerObject.lock()->AddComponent<TiledMapCompatibleCollider2D>();

	topWallsLayerObject.lock()->AddComponent<RigidBody2D>(bodyDef);
}