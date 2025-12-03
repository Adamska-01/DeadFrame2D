#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Actions/RuntimeActionMap.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include <Constants/ResourcePaths.h>
#include <Models/Input/ActionMap/InputActionMapBucket.h>
#include <Tools/Serialization/JsonSerializer.h>


namespace DeadFrame2D::Core
{
	using namespace Shared::Constants;
	using namespace Shared::Models;


	InputActionResolver::InputActionResolver()
	{
		if (!Shared::Tools::IsSerializable<InputActionMapBucket>())
		{
			throw std::runtime_error("[Input] InputActionMapBucket is not serializable. Cannot load input configuration...");
		}

		auto actionMapBucket = Shared::Tools::DeserializeFromFile<InputActionMapBucket>(Paths::Files::INPUT_CONTROLS);

		for (auto& actionMap : actionMapBucket.actionMaps)
		{
			activeActionMaps.push_back(std::make_shared<RuntimeActionMap>(actionMap));
		}
	}

	ActionPhase InputActionResolver::ResolvePhase(bool started, bool held, bool cancelled)
	{
		if (started && !cancelled)
			return ActionPhase::STARTED;
		else if (held && !cancelled)
			return ActionPhase::PERFORMED;
		else if (cancelled)
			return ActionPhase::CANCELED;
		else
			return ActionPhase::WAITING;
	}

	void InputActionResolver::ResolveSimple(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		auto keyState = device.GetKeyState(binding.control.value());

		if (std::holds_alternative<bool>(action.value))
		{
			action.value = (std::get<bool>(action.value) || keyState.value > 0.5f);
		}
		else if (std::holds_alternative<float>(action.value))
		{
			action.value = std::fmax(std::get<float>(action.value), keyState.value);
		}

		action.phase = ResolvePhase(keyState.pressed, keyState.held, keyState.released);
	}

	void InputActionResolver::ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		auto neg = device.GetKeyState(binding.composite1D->negative);
		auto pos = device.GetKeyState(binding.composite1D->positive);

		action.value = pos.value - neg.value;

		action.phase = ResolvePhase(
			pos.pressed || neg.pressed,
			pos.held || neg.held,
			pos.released || neg.released);
	}

	void InputActionResolver::ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		auto up = device.GetKeyState(binding.composite2D->up);
		auto down = device.GetKeyState(binding.composite2D->down);
		auto left = device.GetKeyState(binding.composite2D->left);
		auto right = device.GetKeyState(binding.composite2D->right);

		auto bindingValue = Vector2F(right.value - left.value, up.value - down.value);

		// Combine with existing value (take maximum magnitude to support multiple bindings)
		// This ensures that if multiple bindings are active (e.g., WASD and Arrow keys),
		// we use the one with the strongest input
		auto currentValue = std::get<Vector2F>(action.value);
		auto currentMagnitude = currentValue.Magnitude();
		auto bindingMagnitude = bindingValue.Magnitude();
		
		// Use the binding with the larger magnitude
		if (bindingMagnitude > currentMagnitude)
		{
			action.value = bindingValue;
		}

		// For Composite2D, phase is based on whether there's any input
		// Compare current frame with previous frame to determine phase transitions
		auto finalValue = std::get<Vector2F>(action.value);
		auto hasInput = finalValue.Magnitude() > 0.001f;
		auto hadInput = std::get<Vector2F>(action.previousValue).Magnitude() > 0.001f;

		action.phase = ResolvePhase(
			hasInput && !hadInput,
			hasInput,
			!hasInput && hadInput);
	}

	void InputActionResolver::ProcessAndSend(const std::vector<std::shared_ptr<InputDevice>>& allDevices)
	{
		for (auto& map : activeActionMaps)
		{
			if (!map->IsEnabled())
				continue;

			for (auto& action : map->GetActions())
			{
				action.ResetFrame();

				for (auto& device : allDevices)
				{
					for (const auto& binding : action.GetBindings())
					{
						if (binding.device != device->Type())
							continue;

						switch (binding.type)
						{
							case BindingType::SIMPLE:
								ResolveSimple(*device, action, binding);
								break;

							case BindingType::COMPOSITE_1D:
								ResolveComposite1D(*device, action, binding);
								break;

							case BindingType::COMPOSITE_2D:
								ResolveComposite2D(*device, action, binding);
								break;
						}
					}
				}
			}
		}
	}

	bool InputActionResolver::EnableActionMap(const std::string& name)
	{
		for (size_t i = 0; i < activeActionMaps.size(); i++)
		{
			auto canEnable = activeActionMaps[i]->Name() == name;

			if (canEnable)
			{
				activeActionMaps[i]->Enable();

				return true;
			}
		}

		return false;
	}

	bool InputActionResolver::DisableActionMap(const std::string& name)
	{
		for (size_t i = 0; i < activeActionMaps.size(); i++)
		{
			auto canDisable = activeActionMaps[i]->Name() == name;

			if (canDisable)
			{
				activeActionMaps[i]->Disable();

				return true;
			}
		}

		return false;
	}

	bool InputActionResolver::SwitchToActionMap(const std::string& name)
	{
		auto switched = false;

		for (size_t i = 0; i < activeActionMaps.size(); i++)
		{
			auto canEnable = activeActionMaps[i]->Name() == name;

			if (canEnable)
			{
				activeActionMaps[i]->Enable();

				switched = true;
			}
			else
			{
				activeActionMaps[i]->Disable();
			}
		}

		return switched;
	}

	std::optional<RuntimeInputAction> InputActionResolver::GetActionStateTEST(const std::string actionName)
	{
		for (auto& map : activeActionMaps)
		{
			for (auto& action : map->GetActions())
			{
				if (action.name != actionName)
					continue;

				return action;
			}
		}

		return std::nullopt;
	}
}