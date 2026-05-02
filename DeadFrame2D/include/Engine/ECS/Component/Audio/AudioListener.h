#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"


class b2Body;
class b2Fixture;


namespace DF2D::Engine
{
	class Transform;
	
	
	class DF2D_API AudioListener : public ContactEventProvider
	{
		TYPE_INFO(AudioListener, ContactEventProvider);


	protected:
		ComponentHandle<Transform> transform;

		b2Body* collisionBody;

		b2Fixture* collisionFixture;

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