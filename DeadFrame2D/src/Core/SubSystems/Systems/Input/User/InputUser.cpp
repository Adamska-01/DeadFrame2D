#include "Core/SubSystems/Systems/Input/User/InputUser.h"


namespace DeadFrame2D::Core
{
	InputUser::InputUser(UserID id, const std::string& name)
		: id(id), name(name)
	{
	}

	UserID InputUser::ID() const
	{ 
		return id; 
	}
	
	const std::string& InputUser::Name() const
	{ 
		return name; 
	}

	void InputUser::PairDevice(std::shared_ptr<InputDevice> device)
	{
		if (device == nullptr) 
			return;

		auto itDuplicate = std::find_if(
			devices.begin(), 
			devices.end(), 
			[&](const auto& d) 
			{ 
				return d->ID() == device->ID(); 
			});

		if (itDuplicate == devices.end())
		{
			devices.push_back(device);
		}
	}

	void InputUser::UnpairDevice(DeviceID deviceId)
	{
		devices.erase(
			std::remove_if(
				devices.begin(), 
				devices.end(), 
				[&](const auto& d) 
				{ 
					return d->ID() == deviceId; 
				}), 
			devices.end());
	}

	const std::vector<std::shared_ptr<InputDevice>>& InputUser::Devices() const
	{
		return devices;
	}

	// TODO: Remove
	//InputUser::ActionValue InputUser::EvaluateAction(const ActionMap& map, const std::string& actionName) const
	//{
	//	auto opt = map.GetAction(actionName);
	//	if (!opt) return {};
	//	const auto& action = opt.value();

	//	// For simplicity: for Button -> OR across bindings; Axis -> best non-zero; Vector2 -> combine composite overrides
	//	if (action.type == BindingType::Button)
	//	{
	//		bool result = false;
	//		for (const auto& b : action.bindings)
	//		{
	//			for (const auto& dev : devices_)
	//			{
	//				if (dev->Type() != b.deviceType) continue;
	//				auto s = dev->GetControlState(b.controlId);
	//				if (s.pressed || s.held) { result = true; break; }
	//			}
	//			if (result) break;
	//		}
	//		return ActionValue{ std::variant<std::monostate, bool, float, std::pair<float,float>>(result) };
	//	}
	//	else if (action.type == BindingType::Axis)
	//	{
	//		float best = 0.0f;
	//		for (const auto& b : action.bindings)
	//		{
	//			for (const auto& dev : devices_)
	//			{
	//				if (dev->Type() != b.deviceType) continue;
	//				auto s = dev->GetControlState(b.controlId);
	//				float v = s.value * b.scale;
	//				if (fabs(v) > fabs(best)) best = v;
	//			}
	//		}
	//		return ActionValue{ std::variant<std::monostate, bool, float, std::pair<float,float>>(best) };
	//	}
	//	else if (action.type == BindingType::Vector2)
	//	{
	//		// support composite from keys or analog stick bindings - simple approach:
	//		float x = 0.0f, y = 0.0f;
	//		for (const auto& b : action.bindings)
	//		{
	//			if (b.composite2D)
	//			{
	//				auto comp = b.composite2D.value();
	//				for (const auto& dev : devices_)
	//				{
	//					if (dev->Type() != b.deviceType) continue;
	//					auto up = dev->GetControlState(comp.up).held ? 1.0f : 0.0f;
	//					auto down = dev->GetControlState(comp.down).held ? -1.0f : 0.0f;
	//					auto left = dev->GetControlState(comp.left).held ? -1.0f : 0.0f;
	//					auto right = dev->GetControlState(comp.right).held ? 1.0f : 0.0f;
	//					x += (left + right) * b.scale;
	//					y += (up + down) * b.scale;
	//				}
	//			}
	//			else if (b.bindingType == BindingType::Vector2)
	//			{
	//				// assume controlId refers to axis pair convention (e.g., axis X and axis Y adjacent)
	//				for (const auto& dev : devices_)
	//				{
	//					if (dev->Type() != b.deviceType) continue;
	//					// This requires your ControllerDevice to offer axis pair control ids; for now assume two controlIds: controlId (X), controlId+1 (Y)
	//					auto sx = dev->GetControlState(b.controlId).value;
	//					auto sy = dev->GetControlState(b.controlId + 1).value;
	//					x += sx * b.scale;
	//					y += sy * b.scale;
	//				}
	//			}
	//		}
	//		return ActionValue{ std::variant<std::monostate, bool, float, std::pair<float,float>>(std::pair<float,float>(x,y)) };
	//	}

	//	return {};
	//}
}