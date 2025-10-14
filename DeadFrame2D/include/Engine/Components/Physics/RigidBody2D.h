#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "Data/Physics/BodyType2D.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Delegates/MulticastDelegate.h"


struct b2FixtureDef;
class b2Fixture;
class b2Body;


namespace DeadFrame2D::Engine
{
	class Transform;


	class DF2D_API RigidBody2D : public GameComponent
	{
		TYPE_INFO(RigidBody2D, GameComponent);


	private:
		ComponentHandle<Transform> transform;

		b2Body* body;

		mutable DeadFrame2D::Core::Vector2F velocity;

		mutable DeadFrame2D::Core::Vector2F acceleration;

		DeadFrame2D::Utilities::MultiCastVoid pendingActions;

		DeadFrame2D::Core::Vector2F lastTransformPosition;

		float lastTransformRotation;


	public:
		RigidBody2D(const DeadFrame2D::Data::BodyDefinition2D& bodyDefinition);

		RigidBody2D(RigidBody2D&& other) = default;

		virtual ~RigidBody2D() override;


		void Init() override;

		void Start() override;

		void Update(float deltaTime) override;

		void Draw() override;


		b2Fixture* CreateFixture(const b2FixtureDef* fixtureDef);

		void ChangeBodyType(DeadFrame2D::Data::BodyType2D newBodyType);

		void DestroyFixture(b2Fixture* fixtureDef);


		DeadFrame2D::Core::Vector2F GetVelocity() const;

		void SetVelocity(const DeadFrame2D::Core::Vector2F& velocity);

		void SetVelocityX(float velX);

		void SetVelocityY(float velY);

		void AddImpulse(const DeadFrame2D::Core::Vector2F& impulse);

		void AddImpulseX(float impulseX);

		void AddImpulseY(float impulseY);

		void AddForce(const DeadFrame2D::Core::Vector2F& force);

		void SetGravityScale(float newGravityScale);
	};
}