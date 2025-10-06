#pragma once
#include "Data/Collision/CollisionInfo.h"
#include "DF2D_API.h"
#include "Engine/Components/GameComponent.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <functional>


namespace DeadFrame2D::Engine
{
	class DF2D_API ContactEventProvider : public GameComponent
	{
	protected:
		DeadFrame2D::Utilities::MulticastDelegate<const DeadFrame2D::Data::CollisionInfo&> onContactEnter;

		DeadFrame2D::Utilities::MulticastDelegate<const DeadFrame2D::Data::CollisionInfo&> onContactExit;


	public:
		ContactEventProvider();

		virtual ~ContactEventProvider() override;


		virtual void RegisterContactEnterHandler(const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler, std::uintptr_t identifier);

		virtual void RegisterContactExitHandler(const std::function<void(const DeadFrame2D::Data::CollisionInfo&)>& handler, std::uintptr_t identifier);

		virtual void DeregisterContactEnterHandler(std::uintptr_t identifier);

		virtual void DeregisterContactExitHandler(std::uintptr_t identifier);


		virtual void InvokeCollisionEnter(const DeadFrame2D::Data::CollisionInfo& info);

		virtual void InvokeCollisionExit(const DeadFrame2D::Data::CollisionInfo& info);
	};
}