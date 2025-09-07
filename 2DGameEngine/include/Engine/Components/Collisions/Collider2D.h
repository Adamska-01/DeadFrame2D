#pragma once
#include "Core/Coroutines/Task.h"
#include "Data/Collision/CollisionInfo.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "Engine/Components/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Utilities/Delegates/MulticastDelegate.h"


class b2Fixture;
class Transform;
class RigidBody2D;


class Collider2D : public ContactEventProvider
{
	friend class ContactListener;


private:
	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	Task Disable();


protected:
	b2Fixture* fixture;

	Transform* transform;

	RigidBody2D* rigidBody;

	PhysicsMaterial physicsMaterial;


	Collider2D(const PhysicsMaterial& physicsMaterial);

	virtual ~Collider2D() override;


	virtual void RebuildFixture();


	void SearchRigidBody();


	virtual void OnGameObjectActiveStateChangedHandler(GameObject* obj, bool isActive) override;


public:
	virtual void Init();

	virtual void Start();

	virtual void Update(float dt);

	virtual void Draw();


	bool IsTrigger() const;

	void SetIsTrigger(bool value);


	Transform* GetTranform() const;

	PhysicsMaterial GetPhysicsMaterial();
};