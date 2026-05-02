#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;


	InputUser::InputUser(InputUserID id, const std::string& name)
		: id(id), name(name)
	{
	}

	InputUserID InputUser::ID() const
	{ 
		return id; 
	}
	
	const std::string& InputUser::Name() const
	{ 
		return name; 
	}

	bool InputUser::IsDevicePaired(InputDeviceID deviceID) const
	{
		return Input::Users()->GetUserFromPairedDevice(deviceID) == this;
	}

	const std::vector<InputDeviceID>& InputUser::PairedDevices() const
	{
		return Input::Users()->GetDevicesPairedToUser(id);
	}
}