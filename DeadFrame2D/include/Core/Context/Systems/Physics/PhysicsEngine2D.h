#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Models/Physics/Masks/CollisionMasks.h"
#include "Models/Physics/PhysicsConfig.h"
#include <memory>


struct b2BodyDef;
class b2Body;
class b2World;
class b2ContactListener;
class b2Draw;


namespace DeadFrame2D::Factories
{
	class ColliderDrawer;
}


namespace DeadFrame2D::Core
{
	class DF2D_API PhysicsEngine2D : public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		static PhysicsEngine2D* instance;


		std::unique_ptr<b2World> world;

		std::unique_ptr<b2ContactListener> contactListener;

		std::unique_ptr<Factories::ColliderDrawer> debugDrawer;

		Models::PhysicsConfig physicsConfig;

		Models::CollisionMasks collisionMasks;


		PhysicsEngine2D(const Models::PhysicsConfig& physicsConfig);

		~PhysicsEngine2D() override;

		PhysicsEngine2D(const PhysicsEngine2D&) = delete;

		PhysicsEngine2D(PhysicsEngine2D&&) = delete;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static Vector2F GetGravity();

		static void SetGravity(const Vector2F& newGravity);

		static b2Body* CreateBody(const b2BodyDef* bodyDef);

		static void DestroyBody(b2Body* bodyToDestroy);

		static const Models::PhysicsConfig& GetPhysicsConfig();

		static const Models::CollisionMasks& GetCollisionMasks();
	};
}