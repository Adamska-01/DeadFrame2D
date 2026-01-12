#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;


	InputUserCreatedEvent::InputUserCreatedEvent(DeadFrame2D::Data::InputUserID inputUserID, const std::string& inputUserName)
		: inputUserID(inputUserID),
		inputUserName(inputUserName)

	{
	}

	const std::string& InputUserCreatedEvent::GetInputUserName() const
	{
		return inputUserName;
	}

	DeadFrame2D::Data::InputUserID InputUserCreatedEvent::GetInputUserID() const
	{
		return inputUserID;
	}
}