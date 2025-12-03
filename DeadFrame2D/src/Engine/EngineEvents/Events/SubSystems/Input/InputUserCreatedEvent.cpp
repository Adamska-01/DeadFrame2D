#include "Core/SubSystems/Systems/Input/User/InputUser.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	InputUserCreatedEvent::InputUserCreatedEvent(std::shared_ptr<InputUser> inputUserCreated)
		: inputUserCreated(inputUserCreated)
	{
	}

	std::shared_ptr<const InputUser> InputUserCreatedEvent::GetInputUserCreated() const
	{
		return inputUserCreated;
	}
}