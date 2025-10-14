#pragma once
#include "DF2D_API.h"
#include "Engine/Entity/GameObjectObserver.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API GameComponent : public GameObjectObserver
	{
		friend class ComponentBucket;


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


		virtual void Init() = 0;

		virtual void Start() = 0;

		virtual void Update(float deltaTime) = 0;

		virtual void Draw() = 0;


		virtual void MarkDirty();


		std::weak_ptr<GameObject> GetGameObject() const;

		bool IsActive() const;

		void SetActive(bool value);

		bool IsA(const DeadFrame2D::Core::TypeInfo* type) const;
	};
}