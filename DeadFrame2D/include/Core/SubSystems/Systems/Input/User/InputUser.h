#pragma once
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include <memory>
#include <string>
#include <vector>


namespace DeadFrame2D::Core
{
	using UserID = uint32_t;


	class InputUser
	{
	private:
		UserID id;

		std::string name;

		std::vector<std::shared_ptr<InputDevice>> devices;


	public:
		InputUser(UserID id, const std::string& name = "User");


		UserID ID() const;

		const std::string& Name() const;


		void PairDevice(std::shared_ptr<InputDevice> device);

		void UnpairDevice(DeviceID deviceId);

		const std::vector<std::shared_ptr<InputDevice>>& Devices() const;
	};
}