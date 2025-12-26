#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"


namespace DeadFrame2D::Core
{
	class InputUser;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API InputUserCreatedEvent : public DispatchableEvent
	{
	private:
		const DeadFrame2D::Core::InputUser* inputUserCreated;


	public:
		InputUserCreatedEvent(const DeadFrame2D::Core::InputUser* inputUserCreated);


		const DeadFrame2D::Core::InputUser* GetInputUserCreated() const;
	};
}