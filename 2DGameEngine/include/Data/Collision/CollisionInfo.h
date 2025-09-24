#pragma once
#include "Core/Math/Vector2.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;
}


namespace DeadFrame2D::Data
{
	struct CollisionInfo
	{
		DeadFrame2D::Core::Vector2F contactPoint = DeadFrame2D::Core::Vector2F::Zero;

		DeadFrame2D::Core::Vector2F normal = DeadFrame2D::Core::Vector2F::Zero;

		std::weak_ptr<DeadFrame2D::Engine::GameObject> thisGameObject;

		std::weak_ptr<DeadFrame2D::Engine::GameObject> otherGameObject;
	};
}