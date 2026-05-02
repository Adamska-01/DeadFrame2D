#pragma once
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include <string>


namespace DF2D::Engine
{
	class DF2D_API InputUserCreatedEvent : public DispatchableEvent
	{
	private:
		std::string inputUserName;

		Data::InputUserID inputUserID;


	public:
		InputUserCreatedEvent(Data::InputUserID inputUserID, const std::string& inputUserName);


		const std::string& GetInputUserName() const;

		Data::InputUserID GetInputUserID() const;
	};
}