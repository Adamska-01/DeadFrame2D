#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsBackend.h"
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsContactSink.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Models/Physics/Masks/CollisionMasks.h"
#include "Models/Physics/PhysicsConfig.h"
#include <memory>
#include <unordered_map>


namespace DF2D::Engine
{
	class ContactEventProvider;
}


namespace DF2D::Core
{
	class PhysicsDebugDrawer;


	class DF2D_API PhysicsEngine2D : public ICoreSystem, private IPhysicsContactSink
	{
	private:
		struct FixtureRecord
		{
			Engine::ComponentHandle<Engine::ContactEventProvider> provider;

			Data::BodyID body = 0;
		};


		std::unique_ptr<IPhysicsBackend> backend;

		std::unique_ptr<PhysicsDebugDrawer> debugDrawer;

		Models::PhysicsConfig physicsConfig;

		Models::CollisionMasks collisionMasks;

		std::unordered_map<Data::FixtureID, FixtureRecord> fixtureRecords;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void OnContactBegin(Data::FixtureID fixtureA, Data::FixtureID fixtureB, const Vector2F& contactPoint, const Vector2F& normal) override;

		void OnContactEnd(Data::FixtureID fixtureA, Data::FixtureID fixtureB) override;


	public:
		PhysicsEngine2D(const Models::PhysicsConfig& physicsConfig, Models::CollisionMasks collisionMasks, std::unique_ptr<IPhysicsBackend> backend);

		~PhysicsEngine2D() override;

		PhysicsEngine2D(const PhysicsEngine2D&) = delete;

		PhysicsEngine2D(PhysicsEngine2D&&) = delete;


		Vector2F GetGravity() const;

		void SetGravity(const Vector2F& newGravity);


		Data::BodyID CreateBody(const Data::BodyDefinition2D& bodyDefinition);

		void DestroyBody(Data::BodyID body);

		Data::FixtureID CreateFixture(Data::BodyID body, const Data::PhysicsMaterial& physicsMaterial, const Engine::ComponentHandle<Engine::ContactEventProvider>& contactEventProvider);

		void DestroyFixture(Data::FixtureID fixture);


		void SetBodyEnabled(Data::BodyID body, bool isEnabled);

		void SetBodyType(Data::BodyID body, Data::BodyType2D newBodyType);

		void SetBodyTransform(Data::BodyID body, const Vector2F& position, float angleRad);

		Data::BodyTransform2D GetBodyTransform(Data::BodyID body) const;

		void SetBodyAwake(Data::BodyID body, bool isAwake);

		void SetBodyGravityScale(Data::BodyID body, float gravityScale);

		Vector2F GetLinearVelocity(Data::BodyID body) const;

		void SetLinearVelocity(Data::BodyID body, const Vector2F& velocity);

		void ApplyLinearImpulseToCenter(Data::BodyID body, const Vector2F& impulse);

		void ApplyForceToCenter(Data::BodyID body, const Vector2F& force);


		const Models::PhysicsConfig& GetPhysicsConfig() const;

		const Models::CollisionMasks& GetCollisionMasks() const;


		void SetDebugDrawEnabled(bool isEnabled);
	};
}