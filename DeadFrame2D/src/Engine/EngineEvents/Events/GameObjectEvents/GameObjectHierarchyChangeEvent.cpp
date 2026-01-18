#include "Engine/EngineEvents/Events/GameObjectEvents/GameObjectHierarchyChangeEvent.h"
#include "Engine/Entity/GameObject.h"


namespace DeadFrame2D::Engine
{
	GameObjectHierarchyChangeEvent::GameObjectHierarchyChangeEvent(ObjectHandle<GameObject> obj, ObjectHandle<GameObject> oldParent, ObjectHandle<GameObject> newParent)
		: obj(obj),
		oldParent(oldParent),
		newParent(newParent)
	{
	}


	const ObjectHandle<GameObject>& GameObjectHierarchyChangeEvent::GetObject() const
	{
		return obj;
	}
	
	const ObjectHandle<GameObject>& GameObjectHierarchyChangeEvent::GetOldParent() const
	{
		return oldParent;
	}
	
	const ObjectHandle<GameObject>& GameObjectHierarchyChangeEvent::GetNewParent() const
	{
		return newParent;
	}
}