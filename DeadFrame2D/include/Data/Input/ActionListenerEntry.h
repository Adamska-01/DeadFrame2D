#pragma once
#include "Engine/Entity/ComponentHandleBase.h"
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <string>


namespace DeadFrame2D::Data
{
	struct ActionListenerEntry
	{
		std::string mapName = "default";

		std::string actionName = "default";

		DeadFrame2D::Engine::ComponentHandleBase listener;

		DeadFrame2D::Utilities::ListenerID listenerID = -1;
	};
}