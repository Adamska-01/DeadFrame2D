#pragma once
#include "Data/Components/Collision/CollisionInfo.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DF2D::Core
{
	class PhysicsEngine2D;
}


namespace DF2D::Engine
{
	class DF2D_API ContactEventProvider : public GameComponent
	{
		TYPE_INFO(ContactEventProvider, GameComponent);

		friend class Core::PhysicsEngine2D;


	protected:
		Utilities::MulticastDelegate<const Data::CollisionInfo&> onContactEnter;

		Utilities::MulticastDelegate<const Data::CollisionInfo&> onContactExit;


		virtual void InvokeCollisionEnter(const Data::CollisionInfo& info);

		virtual void InvokeCollisionExit(const Data::CollisionInfo& info);


	public:
		ContactEventProvider();

		virtual ~ContactEventProvider() override;


		void RegisterContactEnterHandler(const ObjectHandleBase& listener, const std::function<void(const Data::CollisionInfo&)>& handler);
		void RegisterContactEnterHandler(const ComponentHandleBase& listener, const std::function<void(const Data::CollisionInfo&)>& handler);

		void RegisterContactExitHandler(const ObjectHandleBase& listener, const std::function<void(const Data::CollisionInfo&)>& handler);
		void RegisterContactExitHandler(const ComponentHandleBase& listener, const std::function<void(const Data::CollisionInfo&)>& handler);

		void DeregisterContactEnterHandler(const ObjectHandleBase& listener);
		void DeregisterContactEnterHandler(const ComponentHandleBase& listener);

		void DeregisterContactExitHandler(const ObjectHandleBase& listener);
		void DeregisterContactExitHandler(const ComponentHandleBase& listener);
	};
}