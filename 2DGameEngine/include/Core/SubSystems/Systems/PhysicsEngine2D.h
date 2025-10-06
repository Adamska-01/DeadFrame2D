#pragma once
#include "Core/Math/Vector2.h"
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include "Models/Physics/Masks/CollisionMasks.h"
#include "Models/Physics/PhysicsConfig.h"
#include <memory>


struct b2BodyDef;
class b2Body;
class b2World;
class b2ContactListener;
class b2Draw;


namespace DeadFrame2D::Core
{
	class DF2D_API PhysicsEngine2D : public ISubSystem
	{
		friend class SubSystems;


	private:
		static PhysicsEngine2D* instance;


		std::unique_ptr<b2World> world;

		std::unique_ptr<b2ContactListener> contactListener;

		std::unique_ptr<b2Draw> debugDrawer;

		Shared::Models::PhysicsConfig physicsConfig;

		Shared::Models::CollisionMasks collisionMasks;


		PhysicsEngine2D(const Shared::Models::PhysicsConfig& physicsConfig);

		virtual ~PhysicsEngine2D() override;

		PhysicsEngine2D(const PhysicsEngine2D&) = delete;

		PhysicsEngine2D(PhysicsEngine2D&&) = delete;


		virtual void Update(float deltaTime) override;

		virtual void BeginFrame() override;

		virtual void EndUpdate() override;

		virtual void EndDraw() override;


	public:
		static Vector2F GetGravity();

		static void SetGravity(const Vector2F& newGravity);

		static b2Body* CreateBody(const b2BodyDef* bodyDef);

		static void DestroyBody(b2Body* bodyToDestroy);

		static const Shared::Models::PhysicsConfig& GetPhysicsConfig();

		static const Shared::Models::CollisionMasks& GetCollisionMasks();
	};
}