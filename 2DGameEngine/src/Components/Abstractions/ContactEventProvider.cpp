#include "Components/Abstractions/ContactEventProvider.h"


ContactEventProvider::ContactEventProvider()
{
	onContactEnter.Clear();
	onContactExit.Clear();
}

ContactEventProvider::~ContactEventProvider()
{
	onContactEnter.Clear();
	onContactExit.Clear();
}

void ContactEventProvider::RegisterContactEnterHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier)
{
	onContactEnter.RegisterCallback(handler, identifier);
}

void ContactEventProvider::RegisterContactExitHandler(const std::function<void(const CollisionInfo&)>& handler, std::uintptr_t identifier)
{
	onContactExit.RegisterCallback(handler, identifier);
}

void ContactEventProvider::DeregisterContactEnterHandler(std::uintptr_t identifier)
{
	onContactEnter.DeregisterCallback(identifier);
}

void ContactEventProvider::DeregisterContactExitHandler(std::uintptr_t identifier)
{
	onContactExit.DeregisterCallback(identifier);
}

void ContactEventProvider::InvokeCollisionEnter(const CollisionInfo& info)
{
	onContactEnter(info);
}

void ContactEventProvider::InvokeCollisionExit(const CollisionInfo& info)
{
	onContactExit(info);
}