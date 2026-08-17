#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/Collision/PhysicsMaterial.h"
#include "Data/Components/Physics/BodyDefinition2D.h"
#include "Data/Components/Physics/BodyType2D.h"
#include "Data/Systems/Physics/BodyID.h"
#include "Data/Systems/Physics/BodyTransform2D.h"
#include "Data/Systems/Physics/FixtureID.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	class IPhysicsContactSink;
	class IPhysicsDebugDraw;


	/**
	 * @brief Backend boundary for the 2D physics simulation.
	 *
	 * Positions, shape dimensions and contact points cross this interface in pixels, angles in radians;
	 * the backend converts to its own unit system. Velocities, impulses, forces and gravity pass through
	 * unconverted (simulation units).
	 */
	class DF2D_API IPhysicsBackend
	{
	public:
		virtual ~IPhysicsBackend() = default;


		virtual void Step(float deltaTime, int velocityIterations, int positionIterations) = 0;

		virtual void SetGravity(const Vector2F& gravity) = 0;


		virtual Data::BodyID CreateBody(const Data::BodyDefinition2D& bodyDefinition) = 0;

		virtual void DestroyBody(Data::BodyID body) = 0;

		virtual Data::FixtureID CreateFixture(Data::BodyID body, const Data::PhysicsMaterial& physicsMaterial) = 0;

		virtual void DestroyFixture(Data::FixtureID fixture) = 0;


		virtual void SetBodyEnabled(Data::BodyID body, bool isEnabled) = 0;

		virtual void SetBodyType(Data::BodyID body, Data::BodyType2D newBodyType) = 0;

		virtual void SetBodyTransform(Data::BodyID body, const Vector2F& position, float angleRad) = 0;

		virtual Data::BodyTransform2D GetBodyTransform(Data::BodyID body) = 0;

		virtual void SetBodyAwake(Data::BodyID body, bool isAwake) = 0;

		virtual void SetBodyGravityScale(Data::BodyID body, float gravityScale) = 0;

		virtual Vector2F GetLinearVelocity(Data::BodyID body) = 0;

		virtual void SetLinearVelocity(Data::BodyID body, const Vector2F& velocity) = 0;

		virtual void ApplyLinearImpulseToCenter(Data::BodyID body, const Vector2F& impulse) = 0;

		virtual void ApplyForceToCenter(Data::BodyID body, const Vector2F& force) = 0;


		virtual void SetContactSink(IPhysicsContactSink* sink) = 0;

		virtual void DebugDraw(IPhysicsDebugDraw& drawer) = 0;
	};
}