#include "Engine/ECS/Component/ComponentContext.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"


namespace DeadFrame2D::Engine
{
	struct ComponentContext::Impl
	{
		ObjectHandle<GameObject> cachedGameObject;
	};


	ComponentContext::ComponentContext()
		: pImpl(std::make_shared<Impl>())
	{
	}

	ComponentContext::~ComponentContext() = default;


	void ComponentContext::SetGameObject(const ObjectHandle<GameObject>& handle)
	{
		rawGameObjectHandle = handle;
		pImpl->cachedGameObject = handle;
	}

	ComponentHandleBase ComponentContext::GetHandle() const
	{
		return selfHandle;
	}

	ObjectHandle<GameObject> ComponentContext::GetGameObject() const
	{
		return pImpl->cachedGameObject;
	}
}