#include "Engine/Events/Context/Input/InputUserDestroyedEvent.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	InputUserDestroyedEvent::InputUserDestroyedEvent(DF2D::Data::InputUserID inputUserID, const std::string& inputUserName)
		: inputUserID(inputUserID),
		inputUserName(inputUserName)

	{
	}

	const std::string& InputUserDestroyedEvent::GetInputUserName() const
	{
		return inputUserName;
	}

	DF2D::Data::InputUserID InputUserDestroyedEvent::GetInputUserID() const
	{
		return inputUserID;
	}
}