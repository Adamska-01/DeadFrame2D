#include "Components/Cannon.h"
#include "Components/PlayerInput.h"
#include "Components/Transform.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/Player.h"
#include <Components/Rendering/Sprite.h>
#include <Math/Vector2.h>


Player::Player()
{
	AddComponent<Sprite>(AssetPaths::Sprites::ARROW_IMAGE_PATH);
	AddComponent<Cannon>();
	AddComponent<PlayerInput>();

	transform->Scale(Vector2F(2.0f, 2.0f));
}

void Player::Init()
{
	GameObject::Init();
}

void Player::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}