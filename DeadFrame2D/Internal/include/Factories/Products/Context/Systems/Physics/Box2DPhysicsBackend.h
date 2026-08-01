#pragma once
#include "Core/Context/Systems/Physics/Abstractions/IPhysicsBackend.h"
#include "Models/Physics/PhysicsConfig.h"
#include <memory>
#include <unordered_map>


class b2Body;
class b2Fixture;
class b2World;


namespace DF2D::Internal
{
	class Box2DContactAdapter;
	class Box2DDebugDrawAdapter;


	class DF2D_API Box2DPhysicsBackend final : public Core::IPhysicsBackend
	{
	private:
		std::unique_ptr<b2World> world;

		std::unique_ptr<Box2DContactAdapter> contactAdapter;

		std::unique_ptr<Box2DDebugDrawAdapter> debugDrawAdapter;

		std::unordered_map<Data::BodyID, b2Body*> bodies;

		std::unordered_map<Data::FixtureID, b2Fixture*> fixtures;

		Data::BodyID nextBodyID;

		Data::FixtureID nextFixtureID;

		float pixelPerMeter;

		float meterPerPixel;


		b2Body* FindBody(Data::BodyID body) const;


	public:
		Box2DPhysicsBackend(const Models::PhysicsConfig& physicsConfig);

		~Box2DPhysicsBackend() override;


		void Step(float deltaTime, int velocityIterations, int positionIterations) override;

		void SetGravity(const Core::Vector2F& gravity) override;


		Data::BodyID CreateBody(const Data::BodyDefinition2D& bodyDefinition) override;

		void DestroyBody(Data::BodyID body) override;

		Data::FixtureID CreateFixture(Data::BodyID body, const Data::PhysicsMaterial& physicsMaterial) override;

		void DestroyFixture(Data::FixtureID fixture) override;


		void SetBodyEnabled(Data::BodyID body, bool isEnabled) override;

		void SetBodyType(Data::BodyID body, Data::BodyType2D newBodyType) override;

		void SetBodyTransform(Data::BodyID body, const Core::Vector2F& position, float angle) override;

		Data::BodyTransform2D GetBodyTransform(Data::BodyID body) override;

		void SetBodyAwake(Data::BodyID body, bool isAwake) override;

		void SetBodyGravityScale(Data::BodyID body, float gravityScale) override;

		Core::Vector2F GetLinearVelocity(Data::BodyID body) override;

		void SetLinearVelocity(Data::BodyID body, const Core::Vector2F& velocity) override;

		void ApplyLinearImpulseToCenter(Data::BodyID body, const Core::Vector2F& impulse) override;

		void ApplyForceToCenter(Data::BodyID body, const Core::Vector2F& force) override;


		void SetContactSink(Core::IPhysicsContactSink* sink) override;

		void DebugDraw(Core::IPhysicsDebugDraw& drawer) override;
	};
}