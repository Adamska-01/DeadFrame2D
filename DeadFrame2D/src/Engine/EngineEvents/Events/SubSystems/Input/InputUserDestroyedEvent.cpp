#include "Core/SubSystems/Systems/Input/User/InputUser.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	InputUserDestroyedEvent::InputUserDestroyedEvent(std::shared_ptr<InputUser> inputUserDestroyed)
		: inputUserDestroyed(inputUserDestroyed)
	{
	}

	std::shared_ptr<const InputUser> InputUserDestroyedEvent::GetInputUserDestroyed() const
	{
		return inputUserDestroyed;
	}
}