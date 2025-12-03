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
	class DF2D_API InputUserCreatedEvent : public DispatchableEvent
	{
	private:
		std::shared_ptr<const DeadFrame2D::Core::InputUser> inputUserCreated;


	public:
		InputUserCreatedEvent(std::shared_ptr<DeadFrame2D::Core::InputUser> inputUserCreated);


		std::shared_ptr<const DeadFrame2D::Core::InputUser> GetInputUserCreated() const;
	};
}