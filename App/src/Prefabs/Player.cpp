#include "Components/Cannon.h"
#include "Components/PlayerInput.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/Player.h"
#include <Components/GameMapParser.h>
#include <Components/Sprite.h>
#include <Components/Transform.h>
#include <Constants/TiledObjectGroupNames.h>
#include <Management/GameObjectRegistry.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>


Player::Player(PlayerIdentifier playerID, Vector2F scale)
	: playerID(playerID)
{
	sprite = nullptr;
	cannon = nullptr;

	transform->Scale(scale);

	AddComponent<Sprite>(AssetPaths::ARROW_IMAGE_PATH);
	AddComponent<Cannon>();
	AddComponent<PlayerInput>();
}

void Player::Init()
{
	GameObject::Init();

	auto gameMapObject = GameObjectRegistry::FindObjectOfType<GameMapParser>();

	if (gameMapObject == nullptr)
	{
		throw std::runtime_error("Cannon::Init: GameMapParser not found in the scene.");
	}

	auto groupObject = gameMapObject->RetrieveObjectGroup(TiledObjectGroupNames::PLAYER_POSITION_GROUP);

	if (!groupObject.has_value() || groupObject.value().points.size() < 0)
		return;

	if (playerID == PlayerIdentifier::PLAYER_1)
	{
		transform->position = groupObject.value().points.front();
	}
	else if (playerID == PlayerIdentifier::PLAYER_2)
	{
		transform->position = groupObject.value().points.back();
	}

	transform->Scale(Vector2F(2.0f, 2.0f));
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
