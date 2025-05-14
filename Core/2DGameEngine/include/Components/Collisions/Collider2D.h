#pragma once
#include "Components/GameComponent.h"
#include "Data/Collision/CollisionInfo.h"
#include "Data/Collision/PhysicsMaterial.h"
#include "EventSystem/DispatchableEvent.h"
#include "Tools/MulticastDelegate.h"


class b2Fixture;
class Transform;
class RigidBody2D;


class Collider2D : public GameComponent
{
	friend class ContactListener;

private:
	void GameObjectCreatedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);

	void GameObjectDestroyedHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent);


protected:
	b2Fixture* fixture;

	Transform* transform;

	RigidBody2D* rigidBody;

	PhysicsMaterial physicsMaterial;

	MulticastDelegate<const CollisionInfo&> OnCollisionEnterCallback;

	MulticastDelegate<const CollisionInfo&> OnCollisionExitCallback;


	Collider2D(const PhysicsMaterial& physicsMaterial);

	virtual ~Collider2D() override;


	virtual void RebuildFixture();

	
	void SearchRigidBody();


public:
	virtual void Init();
	
	virtual void Update(float dt);

	virtual void Draw();


	void RegisterCollisionEnterHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier);

	void RegisterCollisionExitHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier);
	
	void DeregisterCollisionEnterHandler(std::uintptr_t identifier);
	
	void DeregisterCollisionExitHandler(std::uintptr_t identifier);


	Transform* GetTranform() const;

	PhysicsMaterial GetPhysicsMaterial();
};