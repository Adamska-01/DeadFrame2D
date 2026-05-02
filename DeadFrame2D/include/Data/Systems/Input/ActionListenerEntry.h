#pragma once
#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include "Utilities/Delegates/Listeners/ListenerID.h"
#include <string>


namespace DF2D::Data
{
	struct ActionListenerEntry
	{
		std::string mapName = "default";

		std::string actionName = "default";

		Engine::ComponentHandleBase listener;

		Utilities::ListenerID listenerID = -1;
	};
}