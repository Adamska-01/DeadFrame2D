#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API GameObjectHierarchyChangeEvent : public DispatchableEvent
	{
	private:
		ObjectHandle<GameObject> obj;

		ObjectHandle<GameObject> oldParent;

		ObjectHandle<GameObject> newParent;


	public:
		GameObjectHierarchyChangeEvent(
			ObjectHandle<GameObject> obj,
			ObjectHandle<GameObject> oldParent,
			ObjectHandle<GameObject> newParent);


		const ObjectHandle<GameObject>& GetObject() const;

		const ObjectHandle<GameObject>& GetOldParent() const;

		const ObjectHandle<GameObject>& GetNewParent() const;
	};
}