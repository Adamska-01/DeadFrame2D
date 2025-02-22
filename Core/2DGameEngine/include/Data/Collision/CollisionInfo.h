#pragma once
#include "GameObject.h"
#include "Math/Vector2.h"


struct CollisionInfo
{
	Vector2F normal;

	const GameObject* owningGameObject;


	CollisionInfo(Vector2F normal, const GameObject* owningGameObject)
	{
		this->normal = normal;
		this->owningGameObject = owningGameObject;
	}
};