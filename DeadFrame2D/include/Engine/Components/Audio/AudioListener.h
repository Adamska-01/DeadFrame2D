#pragma once
#include "DF2D_API.h"
#include "Engine/Components/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/Entity/ComponentHandle.h"


class b2Body;
class b2Fixture;


namespace DeadFrame2D::Engine
{
	class Transform;
	
	
	class DF2D_API AudioListener : public ContactEventProvider
	{
		TYPE_INFO(AudioListener, ContactEventProvider);


	protected:
		ComponentHandle<Transform> transform;

		b2Body* collisionBody;

		b2Fixture* collisionFixture;

		DeadFrame2D::Core::Vector2F lastTransformPosition;

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