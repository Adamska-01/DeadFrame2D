#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


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

	void ContactEventProvider::RegisterContactEnterHandler(const ObjectHandleBase& listener, const std::function<void(const DF2D::Data::CollisionInfo&)>& handler)
	{
		onContactEnter.AddHandle(listener, handler);
	}

	void ContactEventProvider::RegisterContactEnterHandler(const ComponentHandleBase& listener, const std::function<void(const CollisionInfo&)>& handler)
	{
		onContactEnter.AddHandle(listener, handler);
	}

	void ContactEventProvider::RegisterContactExitHandler(const ObjectHandleBase& listener, const std::function<void(const DF2D::Data::CollisionInfo&)>& handler)
	{
		onContactExit.AddHandle(listener, handler);
	}

	void ContactEventProvider::RegisterContactExitHandler(const ComponentHandleBase& listener, const std::function<void(const CollisionInfo&)>& handler)
	{
		onContactExit.AddHandle(listener, handler);
	}

	void ContactEventProvider::DeregisterContactEnterHandler(const ObjectHandleBase& listener)
	{
		onContactEnter.RemoveByListener(&listener);
	}

	void ContactEventProvider::DeregisterContactEnterHandler(const ComponentHandleBase& listener)
	{
		onContactEnter.RemoveByListener(&listener);
	}

	void ContactEventProvider::DeregisterContactExitHandler(const ObjectHandleBase& listener)
	{
		onContactEnter.RemoveByListener(&listener);
	}

	void ContactEventProvider::DeregisterContactExitHandler(const ComponentHandleBase& listener)
	{
		onContactEnter.RemoveByListener(&listener);
	}

	void ContactEventProvider::InvokeCollisionEnter(const CollisionInfo& info)
	{
		onContactEnter.Broadcast(info);
	}

	void ContactEventProvider::InvokeCollisionExit(const CollisionInfo& info)
	{
		onContactExit.Broadcast(info);
	}
}