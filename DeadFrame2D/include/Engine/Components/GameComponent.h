#pragma once
#include "Core/Reflection/TypeInfo.h"
#include "DF2D_API.h"
#include "Engine/Entity/ComponentHandleBase.h"
#include "Engine/Entity/GameObjectObserver.h"
#include <memory>
#include <type_traits>


namespace DeadFrame2D::Engine
{
	class GameObject;

	template<typename T>
	class ComponentHandle;


	class DF2D_API GameComponent : public GameObjectObserver
	{
		friend class ComponentBucket;


	private:
		ComponentHandleBase selfHandle;


	protected:
		mutable bool isDirty;

		bool isActive;

		std::weak_ptr<GameObject> OwningObject;


	public:
		static inline const DeadFrame2D::Core::TypeInfo StaticTypeInfo{ "GameComponent", nullptr};


		GameComponent();

		GameComponent(GameComponent&& other) = default;

		virtual ~GameComponent() override = default;


		virtual const DeadFrame2D::Core::TypeInfo* GetTypeInfo() const = 0;


		virtual void Init();

		virtual void Start();

		virtual void Update(float deltaTime);

		virtual void LateUpdate(float deltaTime);

		virtual void Draw();


		virtual void MarkDirty();


		std::weak_ptr<GameObject> GetGameObject() const;

		bool IsActive() const;

		void SetActive(bool value);

		bool IsA(const DeadFrame2D::Core::TypeInfo* type) const;


		template<typename T>
		ComponentHandle<T> GetHandleAs() const;
	};
}


namespace DeadFrame2D::Engine
{
	template<typename T>
	inline ComponentHandle<T> GameComponent::GetHandleAs() const
	{
		static_assert(std::is_base_of_v<GameComponent, T>, "T must derive from GameComponent");

		return ComponentHandle<T>::From(selfHandle);
	}
}