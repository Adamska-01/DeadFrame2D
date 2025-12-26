#pragma once
#include "DF2D_API.h"
#include "Engine/EngineEvents/DispatchableEvent.h"


namespace DeadFrame2D::Core
{
	class InputUser;
}


namespace DeadFrame2D::Engine
{
	class DF2D_API InputUserDestroyedEvent : public DispatchableEvent
	{
	private:
		const DeadFrame2D::Core::InputUser* inputUserDestroyed;


	public:
		InputUserDestroyedEvent(const DeadFrame2D::Core::InputUser* inputUserDestroyed);


		const DeadFrame2D::Core::InputUser* GetInputUserDestroyed() const;
	};
}