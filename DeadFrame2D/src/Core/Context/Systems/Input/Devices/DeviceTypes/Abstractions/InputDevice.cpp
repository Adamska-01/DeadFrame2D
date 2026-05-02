#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Core
{
	using namespace DF2D::Utilities;

	
	InputDevice::InputDevice(const char* name, IInputActionHandler* actionHandler)
		: name(name),
		actionHandler(actionHandler)
	{
		Guard::AgainstNull(this->actionHandler, NAME_OF(actionHandler));
	}

	const std::string& InputDevice::Name() const
	{ 
		return name; 
	}
}