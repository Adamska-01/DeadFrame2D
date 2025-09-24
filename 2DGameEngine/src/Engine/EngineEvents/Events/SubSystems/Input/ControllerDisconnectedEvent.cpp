#include "Engine/EngineEvents/Events/SubSystems/Input/ControllerDisconnectedEvent.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;


	ControllerDisconnectedEvent::ControllerDisconnectedEvent(const InputDevice* controllerDevice)
		: controllerDevice(controllerDevice)
	{
	}
}