#pragma once
#include "Core/Context/Systems/Input/User/Abstractions/IUserDevicePairings.h"
#include <unordered_map>


struct MockUserDevicePairings : DF2D::Core::IUserDevicePairings
{
	std::unordered_map<DF2D::Data::InputDeviceID, DF2D::Data::InputUserID> pairings;


	std::optional<DF2D::Data::InputUserID> GetUserIDFromPairedDevice(DF2D::Data::InputDeviceID deviceID) const override
	{
		auto it = pairings.find(deviceID);

		if (it == pairings.end())
			return std::nullopt;

		return it->second;
	}
};