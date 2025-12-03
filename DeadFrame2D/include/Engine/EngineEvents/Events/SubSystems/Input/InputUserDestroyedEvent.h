#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include <memory>


namespace DeadFrame2D::Core
{
	class InputUser;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API InputUserDestroyedEvent : public DispatchableEvent
	{
	private:
		std::shared_ptr<const DeadFrame2D::Core::InputUser> inputUserDestroyed;


	public:
		InputUserDestroyedEvent(std::shared_ptr<DeadFrame2D::Core::InputUser> inputUserDestroyed);


		std::shared_ptr<const DeadFrame2D::Core::InputUser> GetInputUserDestroyed() const;
	};
}