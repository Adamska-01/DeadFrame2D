#include "Constants/TiledObjectGroupNames.h"
#include "GameObjects/Cannon.h"
#include <Components/GameMapParser.h>
#include <Components/Transform.h>
#include <Management/GameObjectRegistry.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>


void Cannon::Init()
{
	GameObject::Init();

	auto gameMapObject = GameObjectRegistry::FindObjectOfType<GameMapParser>();

	if (gameMapObject == nullptr)
	{
		throw std::runtime_error("Cannon::Init: GameMapParser not found in the scene.");
	}

	auto groupObject = gameMapObject->RetrieveObjectGroup(TiledObjectGroupNames::PLAYER_POSITION_GROUP);

	if (!groupObject.has_value() || groupObject.value().points.size() < 1)
		return;

	transform->position = groupObject.value().points.front();
}