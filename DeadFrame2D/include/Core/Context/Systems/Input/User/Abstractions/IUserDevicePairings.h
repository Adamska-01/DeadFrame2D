#pragma once
#include "Data/Systems/Input/InputDeviceID.h"
#include "Data/Systems/Input/InputUserID.h"
#include "DF2D_API.h"
#include <optional>


namespace DF2D::Core
{
	class DF2D_API IUserDevicePairings
	{
	public:
		virtual ~IUserDevicePairings() = default;


		virtual std::optional<Data::InputUserID> GetUserIDFromPairedDevice(Data::InputDeviceID deviceID) const = 0;
	};
}