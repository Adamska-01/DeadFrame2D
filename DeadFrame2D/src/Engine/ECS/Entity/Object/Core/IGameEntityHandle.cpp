#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Core/IGameEntityHandle.h"


namespace DeadFrame2D::Engine
{
	void IGameEntityHandle::SetThisHandle(ObjectHandle<GameObject> thisGameObject)
	{
		this->thisGameObject = thisGameObject;
	}

	ObjectHandle<GameObject> IGameEntityHandle::GetObjectHandle() const
	{
		return thisGameObject;
	}

	ObjectHandle<GameObject> IGameEntityHandle::GetParent() const
	{
		return parent;
	}

	std::vector<ObjectHandle<GameObject>> IGameEntityHandle::GetChildren() const
	{
		return children;
	}
}