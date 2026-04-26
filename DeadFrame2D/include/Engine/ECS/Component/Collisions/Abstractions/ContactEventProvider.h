#pragma once
#include "Data/Collision/CollisionInfo.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/GameComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Engine
{
	class DF2D_API ContactEventProvider : public GameComponent
	{
		TYPE_INFO(ContactEventProvider, GameComponent);


	protected:
		DeadFrame2D::Utilities::MulticastDelegate<const DeadFrame2D::Data::CollisionInfo&> onContactEnter;

		DeadFrame2D::Utilities::MulticastDelegate<const DeadFrame2D::Data::CollisionInfo&> onContactExit;


	public:
		ContactEventProvider();

		virtual ~ContactEventProvider() override;


		void RegisterContactEnterHandler(const ObjectHandleBase& listener, const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler);
		void RegisterContactEnterHandler(const ComponentHandleBase& listener, const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler);

		void RegisterContactExitHandler(const ObjectHandleBase& listener, const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler);
		void RegisterContactExitHandler(const ComponentHandleBase& listener, const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler);

		void DeregisterContactEnterHandler(const ObjectHandleBase& listener);
		void DeregisterContactEnterHandler(const ComponentHandleBase& listener);

		void DeregisterContactExitHandler(const ObjectHandleBase& listener);
		void DeregisterContactExitHandler(const ComponentHandleBase& listener);


		virtual void InvokeCollisionEnter(const DeadFrame2D::Data::CollisionInfo& info);

		virtual void InvokeCollisionExit(const DeadFrame2D::Data::CollisionInfo& info);
	};
}