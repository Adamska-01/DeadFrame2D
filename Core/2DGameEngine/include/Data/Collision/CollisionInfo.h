#pragma once
#include "Math/Vector2.h"
#include <memory>


class GameObject;
class Collider2D;


struct CollisionInfo
{
	Vector2F contactPoint = Vector2F::Zero;

	Vector2F normal = Vector2F::Zero;

	Collider2D* otherCollider = nullptr;

	std::weak_ptr<GameObject> otherGameObject;
};