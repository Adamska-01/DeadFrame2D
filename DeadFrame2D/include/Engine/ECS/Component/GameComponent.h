#pragma once
#include "Core/Reflection/TypeInfo.h"
#include "DF2D_API.h"
#include "Engine/Entity/GameObjectObserver.h"


namespace DeadFrame2D::Engine
{
	class GameObject;

	template<typename T> 
	class ObjectHandle;


	class DF2D_API GameComponent : public GameObjectObserver
	{
	protected:
		mutable bool isDirty;

		bool isActive;


	public:
		static inline const DeadFrame2D::Core::TypeInfo StaticTypeInfo{ "GameComponent", nullptr };


		GameComponent();

		GameComponent(GameComponent&& other) = default;

		virtual ~GameComponent() override;


		virtual const DeadFrame2D::Core::TypeInfo* GetTypeInfo() const = 0;


		virtual void Init();

		virtual void Start();

		virtual void Update(float deltaTime);

		virtual void LateUpdate(float deltaTime);

		virtual void Draw();


		virtual void MarkDirty();


		bool IsActive() const;

		void SetActive(bool value);

		bool IsA(const DeadFrame2D::Core::TypeInfo* type) const;
	};
}