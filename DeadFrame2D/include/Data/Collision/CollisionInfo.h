#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"


namespace DeadFrame2D::Data
{
	struct CollisionInfo
	{
		DeadFrame2D::Core::Vector2F contactPoint = DeadFrame2D::Core::Vector2F::Zero;

		DeadFrame2D::Core::Vector2F normal = DeadFrame2D::Core::Vector2F::Zero;

		DeadFrame2D::Engine::ObjectHandle<DeadFrame2D::Engine::GameObject> thisGameObject;

		DeadFrame2D::Engine::ObjectHandle<DeadFrame2D::Engine::GameObject> otherGameObject;
	};
}