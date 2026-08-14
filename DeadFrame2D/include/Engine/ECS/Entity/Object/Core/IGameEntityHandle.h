#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include <vector>


namespace DF2D::Engine
{
	class GameObject;


	/**
	 * @brief Interface providing access to a GameObject's handle and its hierarchical relationships.
	 *
	 * This interface defines the minimal contract for objects that can expose their
	 * GameObject handle and information about their parent and child relationships
	 * within the scene hierarchy.
	 */
	class DF2D_API IGameEntityHandle
	{
	protected:
		void SetThisHandle(ObjectHandle<GameObject> thisGameObject);


		ObjectHandle<GameObject> thisGameObject;

		ObjectHandle<GameObject> parent;

		std::vector<ObjectHandle<GameObject>> children;


	public:
		/**
		 * @brief Virtual destructor to allow proper cleanup through interface pointers.
		 */
		virtual ~IGameEntityHandle() = default;


		/**
		 * @brief Gets the handle representing this GameObject.
		 * @return The handle of this GameObject.
		 */
		ObjectHandle<GameObject> GetObjectHandle() const;

		/**
		 * @brief Gets the parent GameObject handle, if any.
		 * @return The handle of the parent GameObject, or an empty handle if there is none.
		 */
		ObjectHandle<GameObject> GetParent() const;

		/**
		 * @brief Gets all child GameObject handles associated with this GameObject.
		 * @return A vector containing handles to all child GameObjects.
		 */
		std::vector<ObjectHandle<GameObject>> GetChildren() const;
	};
}