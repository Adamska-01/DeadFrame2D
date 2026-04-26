#include "Engine/Events/Context/Input/InputUserDestroyedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Data;


	InputUserDestroyedEvent::InputUserDestroyedEvent(DeadFrame2D::Data::InputUserID inputUserID, const std::string& inputUserName)
		: inputUserID(inputUserID),
		inputUserName(inputUserName)

	{
	}

	const std::string& InputUserDestroyedEvent::GetInputUserName() const
	{
		return inputUserName;
	}

	DeadFrame2D::Data::InputUserID InputUserDestroyedEvent::GetInputUserID() const
	{
		return inputUserID;
	}
}