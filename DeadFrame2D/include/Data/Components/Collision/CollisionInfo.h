#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"


namespace DF2D::Data
{
	struct CollisionInfo
	{
		Core::Vector2F contactPoint = Core::Vector2F::Zero;

		Core::Vector2F normal = Core::Vector2F::Zero;

		Engine::ObjectHandle<Engine::GameObject> thisGameObject;

		Engine::ObjectHandle<Engine::GameObject> otherGameObject;
	};
}