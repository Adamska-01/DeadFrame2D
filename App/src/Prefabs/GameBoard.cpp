#include "Components/BobbleGrid.h"
#include "Components/GameFlow/GameManager.h"
#include "Components/GridCeiling.h"
#include "Prefabs/GameBoard.h"
#include "Prefabs/GroundDestroyTrigger.h"
#include "Prefabs/Player.h"
#include <Components/Collisions/BoxCollider2D.h>
#include <Components/Physics/RigidBody2D.h>


void GameBoard::ConstructGameObject()
{
	scoreManager = AddComponent<GameManager>();

	auto bodyDef = BodyDefinition2D();
	bodyDef.type = BodyType2D::Static;

	// Bobble Grid
	auto bobbleGridObject = GameObject::Instantiate<GameObject>();
	auto bobbleGridComponent = bobbleGridObject.lock()->AddComponent<BobbleGrid>();

	AddChildGameObject(bobbleGridObject);

	auto gridCeilingObject = GameObject::Instantiate<GameObject>();
	gridCeilingObject.lock()->AddComponent<BoxCollider2D>(Vector2F::One, Vector2F::One, 0.0f);
	gridCeilingObject.lock()->AddComponent<RigidBody2D>(bodyDef);
	gridCeilingObject.lock()->AddComponent<GridCeiling>();

	bobbleGridObject.lock()->AddChildGameObject(gridCeilingObject);


	// Player
	AddChildGameObject(GameObject::Instantiate<Player>());


	// GroundDestroyTrigger
	AddChildGameObject(GameObject::Instantiate<GroundDestroyTrigger>());
	
	
	// UI
}

GameManager* GameBoard::GetScoreManager()
{
	return scoreManager;
}