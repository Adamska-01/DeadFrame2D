#pragma once
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include <string>


namespace DeadFrame2D::Engine
{
	class DF2D_API InputUserCreatedEvent : public DispatchableEvent
	{
	private:
		std::string inputUserName;

		DeadFrame2D::Data::InputUserID inputUserID;


	public:
		InputUserCreatedEvent(DeadFrame2D::Data::InputUserID inputUserID, const std::string& inputUserName);


		const std::string& GetInputUserName() const;

		DeadFrame2D::Data::InputUserID GetInputUserID() const;
	};
}