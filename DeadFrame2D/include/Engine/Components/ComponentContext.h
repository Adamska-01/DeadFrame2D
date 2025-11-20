#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandleBase.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandleBase.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	/**
	 * @brief Backend container that stores component metadata such as its handles,
	 */
	class DF2D_API ComponentContext
	{
		friend class ComponentBucket;


	private:
		ComponentHandleBase selfHandle;

		ObjectHandleBase rawGameObjectHandle;

		struct Impl;
		std::shared_ptr<Impl> pImpl;


		/**
		 * @brief Internal helper to assign the GameObject handle and propagate caches.
		 */
		void SetGameObject(const ObjectHandle<GameObject>& handle);


	public:
		ComponentContext();

		virtual ~ComponentContext();


		/**
		 * @brief Returns the base component handle.
		 */
		ComponentHandleBase GetHandle() const;

		/**
		 * @brief Returns a typed component handle.
		 * @tparam T The component type.
		 */
		template<typename T>
		ComponentHandle<T> GetHandleAs() const;

		/**
		 * @brief Returns the associated GameObject.
		 */
		ObjectHandle<GameObject> GetGameObject() const;
	};
}


namespace DeadFrame2D::Engine
{
	template<typename T>
	ComponentHandle<T> ComponentContext::GetHandleAs() const
	{
		static_assert(std::is_base_of_v<ComponentContext, T>,
			"T must derive from ComponentContext");

		return ComponentHandle<T>::From(selfHandle);
	}
}