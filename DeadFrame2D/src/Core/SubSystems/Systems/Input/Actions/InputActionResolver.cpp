#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Actions/RuntimeActionMap.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Data/Input/InputControlState.h"
#include <Constants/ResourcePaths.h>
#include <Models/Input/ActionMap/InputActionMapBucket.h>
#include <Tools/Serialization/JsonSerializer.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Data;

	using namespace Shared::Constants;
	using namespace Shared::Models;


	InputActionResolver::InputActionResolver()
	{
		if (!Shared::Tools::IsSerializable<InputActionMapBucket>())
		{
			throw std::runtime_error("[Input] InputActionMapBucket is not serializable. Cannot load input configuration...");
		}

		auto actionMapBucket = Shared::Tools::DeserializeFromFile<InputActionMapBucket>(Paths::Files::INPUT_CONTROLS);

		// Construct runtime action map
		for (auto& actionMap : actionMapBucket.actionMaps)
		{
			runtimeActionMaps.push_back(std::make_shared<RuntimeActionMap>(actionMap));
		}

		// Construct map for fast action lookup
		for (auto& map : runtimeActionMaps)
		{
			for (auto& action : map->GetActions())
			{
				for (auto& binding : action.bindings)
				{
					const auto& mapName = map->Name();
					const auto device = binding.device;

					switch (binding.type)
					{
						case BindingType::SIMPLE:
						{
							auto key = std::make_tuple(mapName, device, binding.control.value());

							fastLookupActionMaps[key].push_back({ &action, &binding });

							break;
						}

						case BindingType::COMPOSITE_1D:
						{
							auto keyNeg = std::make_tuple(mapName, device, binding.composite1D->negative);
							auto keyPos = std::make_tuple(mapName, device, binding.composite1D->positive);

							fastLookupActionMaps[keyNeg].push_back({ &action, &binding });
							fastLookupActionMaps[keyPos].push_back({ &action, &binding });

							break;
						}

						case BindingType::COMPOSITE_2D:
						{
							auto keyUp = std::make_tuple(mapName, device, binding.composite2D->up);
							auto keyDown = std::make_tuple(mapName, device, binding.composite2D->down);
							auto keyLeft = std::make_tuple(mapName, device, binding.composite2D->left);
							auto keyRight = std::make_tuple(mapName, device, binding.composite2D->right);

							fastLookupActionMaps[keyUp].push_back({ &action, &binding });
							fastLookupActionMaps[keyDown].push_back({ &action, &binding });
							fastLookupActionMaps[keyLeft].push_back({ &action, &binding });
							fastLookupActionMaps[keyRight].push_back({ &action, &binding });

							break;
						}
					}
				}
			}
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
			if (!action.isValuePending)
			{
				action.pendingValue = keyState.value > 0.5f;
				action.isValuePending = true;
			}
			else
			{
				action.pendingValue = std::get<bool>(action.pendingValue) || (keyState.value > 0.5f);
			}
		}
		else if (std::holds_alternative<float>(action.value))
		{
			if (!action.isValuePending)
			{
				action.pendingValue = keyState.value;
				action.isValuePending = true;
			}
			else
			{
				action.pendingValue = std::max(std::get<float>(action.pendingValue), keyState.value);
			}
		}
	}

	void InputActionResolver::ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		auto neg = device.GetKeyState(binding.composite1D->negative);
		auto pos = device.GetKeyState(binding.composite1D->positive);

		auto value = pos.value - neg.value;

		if (!action.isValuePending)
		{
			action.pendingValue = value;
			action.isValuePending = true;
		}
		else if (std::abs(value) > std::abs(std::get<float>(action.pendingValue)))
		{
			action.pendingValue = value;
		}
	}

	void InputActionResolver::ResolveComposite2D(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		auto up = device.GetKeyState(binding.composite2D->up);
		auto down = device.GetKeyState(binding.composite2D->down);
		auto left = device.GetKeyState(binding.composite2D->left);
		auto right = device.GetKeyState(binding.composite2D->right);

		auto bindingValue = Vector2F(right.value - left.value, up.value - down.value);

		if (!action.isValuePending)
		{
			action.pendingValue = bindingValue;
			action.isValuePending = true;
		}
		else if (bindingValue.Magnitude() > std::get<Vector2F>(action.pendingValue).Magnitude())
		{
			action.pendingValue = bindingValue;
		}
	}

	void InputActionResolver::BeginFrame()
	{
		for (auto& action : activeActions)
		{
			action->ResetFrame();
		}

		activeActions.clear();
	}

	void InputActionResolver::ProcessBinding(const InputDevice& device, int controlID)
	{
		for (auto& map : runtimeActionMaps)
		{
			if (!map->IsEnabled())
				continue;

			auto key = std::make_tuple(map->Name(), device.Type(), controlID);
			auto it = fastLookupActionMaps.find(key);

			if (it == fastLookupActionMaps.end())
				continue;

			for (auto& record : it->second)
			{
				activeActions.insert(record.action);

				switch (record.binding->type)
				{
					case BindingType::SIMPLE:
						ResolveSimple(device, *record.action, *record.binding);
						break;

					case BindingType::COMPOSITE_1D:
						ResolveComposite1D(device, *record.action, *record.binding);
						break;

					case BindingType::COMPOSITE_2D:
						ResolveComposite2D(device, *record.action, *record.binding);
						break;
				}
			}
		}
	}

	void InputActionResolver::FinalizeActions()
	{
		for (auto* action : activeActions)
		{
			action->value = action->isValuePending ? action->pendingValue : action->value;

			if (std::holds_alternative<float>(action->value))
			{
				auto cur = std::get<float>(action->value);
				auto prev = std::get<float>(action->previousValue);

				auto started = (cur > 0.001f) && !(prev > 0.001f);
				auto held = (cur > 0.001f) && (prev > 0.001f);
				auto cancelled = !(cur > 0.001f) && (prev > 0.001f);

				action->phase = ResolvePhase(started, held, cancelled);
			}
			else if (std::holds_alternative<bool>(action->value))
			{
				auto cur = std::get<bool>(action->value);
				auto prev = std::get<bool>(action->previousValue);

				action->phase = ResolvePhase(cur && !prev, cur && prev, !cur && prev);
			}
			else if (std::holds_alternative<Vector2F>(action->value))
			{
				const auto& cur = std::get<Vector2F>(action->value);
				const auto& prev = std::get<Vector2F>(action->previousValue);

				auto hasInput = cur.Magnitude() > 0.001f;
				auto hadInput = prev.Magnitude() > 0.001f;

				action->phase = ResolvePhase(hasInput && !hadInput, hasInput && hadInput, !hasInput && hadInput);
			}
		}
	}

	bool InputActionResolver::EnableActionMap(const std::string& name)
	{
		for (size_t i = 0; i < runtimeActionMaps.size(); i++)
		{
			auto canEnable = runtimeActionMaps[i]->Name() == name;

			if (canEnable)
			{
				runtimeActionMaps[i]->Enable();

				return true;
			}
		}

		return false;
	}

	bool InputActionResolver::DisableActionMap(const std::string& name)
	{
		for (size_t i = 0; i < runtimeActionMaps.size(); i++)
		{
			auto canDisable = runtimeActionMaps[i]->Name() == name;

			if (canDisable)
			{
				runtimeActionMaps[i]->Disable();

				return true;
			}
		}

		return false;
	}

	bool InputActionResolver::SwitchToActionMap(const std::string& name)
	{
		auto switched = false;

		for (size_t i = 0; i < runtimeActionMaps.size(); i++)
		{
			auto canEnable = runtimeActionMaps[i]->Name() == name;

			if (canEnable)
			{
				runtimeActionMaps[i]->Enable();

				switched = true;
			}
			else
			{
				runtimeActionMaps[i]->Disable();
			}
		}

		return switched;
	}

	std::optional<RuntimeInputAction> InputActionResolver::GetActionStateTEST(const std::string actionName)
	{
		for (auto& map : runtimeActionMaps)
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