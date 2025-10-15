#pragma once
#include "Core/Coroutines/Task.h"
#include "Data/Collision/CollisionInfo.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "DF2D_API.h"
#include "Engine/Components/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/Entity/ComponentHandle.h"
#include "Utilities/Delegates/MulticastDelegate.h"


class b2Fixture;


namespace DeadFrame2D::Engine
{
	class Transform;
	class RigidBody2D;


	class DF2D_API Collider2D : public ContactEventProvider
	{
		TYPE_INFO(Collider2D, ContactEventProvider);
		
		
		friend class ContactListener;


	protected:
		b2Fixture* fixture;

		ComponentHandle<Transform> transform;

		ComponentHandle<RigidBody2D> rigidBody;

		DeadFrame2D::Data::PhysicsMaterial physicsMaterial;


		Collider2D(const DeadFrame2D::Data::PhysicsMaterial& physicsMaterial);

		virtual ~Collider2D() override;


		virtual void RebuildFixture();


		void SearchRigidBody();


	public:
		virtual void Init();

		virtual void Update(float dt);


		bool IsTrigger() const;

		void SetIsTrigger(bool value);


		ComponentHandle<Transform> GetTranform() const;

		const DeadFrame2D::Data::PhysicsMaterial& GetPhysicsMaterial();
	};
}