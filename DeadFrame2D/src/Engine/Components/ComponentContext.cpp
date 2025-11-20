#include "Engine/Components/ComponentContext.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"
#include "Engine/Entity/GameObject.h"


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