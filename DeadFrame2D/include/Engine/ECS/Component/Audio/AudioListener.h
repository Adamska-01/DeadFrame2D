#pragma once
#include "Data/Systems/Physics/BodyID.h"
#include "Data/Systems/Physics/FixtureID.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


namespace DF2D::Core
{
	class PhysicsEngine2D;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API AudioListener : public ContactEventProvider
	{
		TYPE_INFO(AudioListener, ContactEventProvider);


	protected:
		Core::PhysicsEngine2D* physicsEngine;

		ComponentHandle<Transform> transform;

		Data::BodyID collisionBody;

		Data::FixtureID collisionFixture;

		Core::Vector2F lastTransformPosition;

		float lastTransformRotation;


		void RebuildFixture();


	public:
		AudioListener();

		virtual ~AudioListener() override;


		virtual void Init() override;

		virtual void Update(float deltaTime) override;

		virtual void LateUpdate(float deltaTime) override;

		virtual void Draw() override;
	};
}