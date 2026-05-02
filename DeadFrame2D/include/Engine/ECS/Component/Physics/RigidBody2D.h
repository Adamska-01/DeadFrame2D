#pragma once
#include "Core/Context/Systems/Coroutines/Task.h"
#include "Core/Math/Vector2.h"
#include "Data/Components/Physics/BodyDefinition2D.h"
#include "Data/Components/Physics/BodyType2D.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Utilities/Delegates/MulticastDelegate.h"


struct b2FixtureDef;
class b2Fixture;
class b2Body;


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API RigidBody2D : public GameComponent
	{
		TYPE_INFO(RigidBody2D, GameComponent);


	private:
		ComponentHandle<Transform> transform;

		b2Body* body;

		mutable Core::Vector2F velocity;

		mutable Core::Vector2F acceleration;

		Utilities::MultiCastVoid pendingActions;

		Core::Vector2F lastTransformPosition;

		float lastTransformRotation;


	protected:
		virtual void OnGameObjectActiveStateChangedHandler(const ObjectHandle<GameObject>& obj, bool isActive) override;

		Core::Task SetEnabled(bool isEnabled);


	public:
		RigidBody2D(const Data::BodyDefinition2D& bodyDefinition);

		RigidBody2D(RigidBody2D&& other) = default;

		virtual ~RigidBody2D() override;


		void Init() override;

		void Update(float deltaTime) override;

		void LateUpdate(float deltaTime) override;


		b2Fixture* CreateFixture(const b2FixtureDef* fixtureDef);

		void ChangeBodyType(Data::BodyType2D newBodyType);

		void DestroyFixture(b2Fixture* fixtureDef);


		Core::Vector2F GetVelocity() const;

		void SetVelocity(const Core::Vector2F& velocity);

		void SetVelocityX(float velX);

		void SetVelocityY(float velY);

		void AddImpulse(const Core::Vector2F& impulse);

		void AddImpulseX(float impulseX);

		void AddImpulseY(float impulseY);

		void AddForce(const Core::Vector2F& force);

		void SetGravityScale(float newGravityScale);
	};
}