#pragma once
#include "Math/Vector2.h"


class GameObject;
class Collider2D;


struct CollisionInfo
{
	Vector2F contactPoint = Vector2F::Zero;

	Vector2F normal = Vector2F::Zero;

	Collider2D* otherCollider = nullptr;

	GameObject* otherGameObject = nullptr;
};