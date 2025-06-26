#pragma once
#include "Components/GameComponent.h"
#include "Data/Collision/CollisionInfo.h"
#include "Tools/MulticastDelegate.h"
#include <functional>


class ContactEventProvider : public GameComponent
{
protected:
	MulticastDelegate<const CollisionInfo&> onContactEnter;

	MulticastDelegate<const CollisionInfo&> onContactExit;


public:
	ContactEventProvider();

	virtual ~ContactEventProvider() override;


	virtual void RegisterContactEnterHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier);

	virtual void RegisterContactExitHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier);

	virtual void DeregisterContactEnterHandler(std::uintptr_t identifier);

	virtual void DeregisterContactExitHandler(std::uintptr_t identifier);


	virtual void InvokeCollisionEnter(const CollisionInfo& info);

	virtual void InvokeCollisionExit(const CollisionInfo& info);
};