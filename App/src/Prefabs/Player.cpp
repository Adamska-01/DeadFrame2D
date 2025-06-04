#include "Components/Cannon.h"
#include "Components/Transform.h"
#include "Constants/AssetPaths.h"
#include "Prefabs/Player.h"
#include <Components/Rendering/Sprite.h>
#include <Math/Vector2.h>


void Player::ConstructGameObject()
{
	AddComponent<Sprite>(AssetPaths::Sprites::ARROW_IMAGE_PATH);
	AddComponent<Cannon>();

	transform->Scale(Vector2F(2.0f, 2.0f));
}