#include "Core/SubSystems/Systems/Input/User/InputUser.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	InputUserCreatedEvent::InputUserCreatedEvent(const InputUser* inputUserCreated)
		: inputUserCreated(inputUserCreated)
	{
	}

	const InputUser* InputUserCreatedEvent::GetInputUserCreated() const
	{
		return inputUserCreated;
	}
}