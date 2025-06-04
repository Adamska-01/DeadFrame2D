#include "Components/BobbleGrid.h"
#include "Components/GameFlow/GameManager.h"
#include "Components/GameMapParser.h"
#include "Constants/GameMapLayerNames.h"
#include "Prefabs/GameBoard.h"
#include "Prefabs/GroundDestroyTrigger.h"
#include "Prefabs/Player.h"
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>


GameBoard::GameBoard(std::string_view mapSource)
	: mapSource(mapSource)
{
}

void GameBoard::ConstructGameObject()
{
	AddComponent<GameManager>();

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


	// Bobble Grid
	auto bobbleGridObject = GameObject::Instantiate<GameObject>();
	auto bobbleGridComponent = bobbleGridObject.lock()->AddComponent<BobbleGrid>(topWallCollider);

	AddChildGameObject(bobbleGridObject);


	// Player
	AddChildGameObject(GameObject::Instantiate<Player>());


	// GroundDestroyTrigger
	AddChildGameObject(GameObject::Instantiate<GroundDestroyTrigger>());


	// UI
}