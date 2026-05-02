#include "Engine/Events/Context/Input/InputUserCreatedEvent.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	InputUserCreatedEvent::InputUserCreatedEvent(DF2D::Data::InputUserID inputUserID, const std::string& inputUserName)
		: inputUserID(inputUserID),
		inputUserName(inputUserName)

	{
	}

	const std::string& InputUserCreatedEvent::GetInputUserName() const
	{
		return inputUserName;
	}

	DF2D::Data::InputUserID InputUserCreatedEvent::GetInputUserID() const
	{
		return inputUserID;
	}
}