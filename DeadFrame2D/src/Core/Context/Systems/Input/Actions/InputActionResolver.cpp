#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Actions/RuntimeActionMap.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Data/Systems/Input/InputControlState.h"


namespace DF2D::Core
{
	using namespace DF2D::Data;
	using namespace DF2D::Engine;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	InputActionResolver::InputActionResolver(InputActionMapBucket actionMapBucket, const IUserDevicePairings& userPairings)
		: actionMapBucket(std::move(actionMapBucket)),
		userPairings(&userPairings)
	{
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
		auto simpleID = std::get<int>(binding.input.value);
		auto controlType = binding.input.controlType;

		auto state = controlType == InputControlType::DIGITAL
			? device.GetButtonState(simpleID)
			: device.GetAxisState(simpleID);

		if (std::holds_alternative<bool>(action.value))
		{
			if (!action.isValuePending)
			{
				action.pendingValue = state.value > 0.5f;
				action.isValuePending = true;
			}
			else
			{
				action.pendingValue = std::get<bool>(action.pendingValue) || (state.value > 0.5f);
			}
		}
		else if (std::holds_alternative<float>(action.value))
		{
			if (!action.isValuePending)
			{
				action.pendingValue = state.value;
				action.isValuePending = true;
			}
			else
			{
				action.pendingValue = std::max(std::get<float>(action.pendingValue), state.value);
			}
		}
	}

	void InputActionResolver::ResolveComposite1D(const InputDevice& device, RuntimeInputAction& action, const Binding& binding)
	{
		const auto& comp = std::get<Composite1D>(binding.input.value);
		auto controlType = binding.input.controlType;

		auto neg = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.negative)
			: device.GetAxisState(comp.negative);
		auto pos = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.positive)
			: device.GetAxisState(comp.positive);

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
		const auto& comp = std::get<Composite2D>(binding.input.value);
		auto controlType = binding.input.controlType;

		auto up = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.up)
			: device.GetAxisState(comp.up);
		auto down = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.down)
			: device.GetAxisState(comp.down);
		auto left = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.left)
			: device.GetAxisState(comp.left);
		auto right = controlType == InputControlType::DIGITAL
			? device.GetButtonState(comp.right)
			: device.GetAxisState(comp.right);

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

	void InputActionResolver::AddUser(InputUserID userID)
	{
		// Create empty slots
		runtimeActionMaps[userID] = {};
		activeActions[userID] = {};

		auto& runtimeMapsForBucket = runtimeActionMaps[userID];

		// --- Construct runtime action maps ---
		for (auto& actionMap : actionMapBucket.actionMaps)
		{
			runtimeMapsForBucket.maps.push_back(std::make_shared<RuntimeActionMap>(actionMap));
		}

		// --- Build name-to-index lookup ---
		for (size_t i = 0; i < runtimeMapsForBucket.maps.size(); ++i)
		{
			runtimeMapsForBucket.nameToIndex[runtimeMapsForBucket.maps[i]->Name()] = i;
		}
	}

	void InputActionResolver::RemoveUser(InputUserID userID)
	{
		if (auto it = activeActions.find(userID); it != activeActions.end())
		{
			for (auto* action : it->second)
			{
				callableActions.erase(action);
			}
		}

		runtimeActionMaps.erase(userID);
		activeActions.erase(userID);
	}

	void InputActionResolver::BeginFrame()
	{
		for (auto& [userID, actionSet] : activeActions)
		{
			for (auto action : actionSet)
			{
				action->ResetFrame();
			}
		}

		activeActions.clear();
	}

	void InputActionResolver::PreUpdate()
	{
		for (auto& [userID, userActions] : activeActions)
		{
			for (auto* action : userActions)
			{
				// Commit pending value if any
				if (action->isValuePending)
				{
					action->value = action->pendingValue;
					action->ApplyProcessors();
				}

				std::visit(
					[&](auto& val)
					{
						using T = std::decay_t<decltype(val)>;

						T prev = std::get<T>(action->previousValue);

						auto started = false, held = false, cancelled = false;

						// --- FLOAT ---
						if constexpr (std::is_same_v<T, float>)
						{
							started = (val > 0.001f) && !(prev > 0.001f);
							held = (val > 0.001f) && (prev > 0.001f);
							cancelled = !(val > 0.001f) && (prev > 0.001f);
						}
						// --- BOOL ---
						else if constexpr (std::is_same_v<T, bool>)
						{
							started = val && !prev;
							held = val && prev;
							cancelled = !val && prev;
						}
						// --- VECTOR2 ---
						else if constexpr (std::is_same_v<T, Vector2F>)
						{
							auto mag = val.Magnitude();
							auto prevMag = prev.Magnitude();

							started = mag > 0.001f && !(prevMag > 0.001f);
							held = mag > 0.001f && (prevMag > 0.001f);
							cancelled = !(mag > 0.001f) && (prevMag > 0.001f);
						}

						action->phase = ResolvePhase(started, held, cancelled);
					},
					action->value);

				if (action->phase == ActionPhase::STARTED)
				{
					callableActions.insert(action);
				}
			}
		}

		for (auto it = callableActions.begin(); it != callableActions.end(); )
		{
			auto* action = *it;

			action->listeners.Broadcast(InputActionView(action->phase, action->value));

			if (action->phase == ActionPhase::CANCELED)
			{
				it = callableActions.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void InputActionResolver::ProcessBinding(const InputDevice& device, InputControlType inputControlType, int controlID)
	{
		auto userID = userPairings->GetUserIDFromPairedDevice(device.ID());
		if (!userID.has_value())
			return;

		auto mapsIt = runtimeActionMaps.find(*userID);
		if (mapsIt == runtimeActionMaps.end())
			return;

		auto& userActiveActions = activeActions[*userID];

		for (auto& map : mapsIt->second.maps)
		{
			if (!map->IsEnabled())
				continue;

			for (auto& action : map->GetActions())
			{
				for (auto& binding : action.bindings)
				{
					if (binding.input.inputDeviceType != device.Type()
						|| binding.input.controlType != inputControlType)
						continue;

					auto matches = false;

					switch (binding.input.bindingType)
					{
					case BindingType::SIMPLE:
						matches = std::get<int>(binding.input.value) == controlID;
						break;

					case BindingType::COMPOSITE_1D:
					{
						auto& comp = std::get<Composite1D>(binding.input.value);
						matches = comp.negative == controlID || comp.positive == controlID;
						break;
					}

					case BindingType::COMPOSITE_2D:
					{
						auto& comp = std::get<Composite2D>(binding.input.value);
						matches = comp.up == controlID
							|| comp.down == controlID
							|| comp.left == controlID
							|| comp.right == controlID;
						break;
					}
					}

					if (matches)
					{
						userActiveActions.insert(&action);

						switch (binding.input.bindingType)
						{
						case BindingType::SIMPLE:
							ResolveSimple(device, action, binding);
							break;
						case BindingType::COMPOSITE_1D:
							ResolveComposite1D(device, action, binding);
							break;
						case BindingType::COMPOSITE_2D:
							ResolveComposite2D(device, action, binding);
							break;
						}
					}
				}
			}
		}
	}

	ListenerID InputActionResolver::RegisterAction(
		InputUserID userID,
		const std::string& actionMapName,
		const std::string& actionName,
		const ComponentHandleBase& listener,
		const std::function<void(const InputActionView&)>& handler)
	{
		if (!listener.IsValid())
			return -1;

		auto mapsIt = runtimeActionMaps.find(userID);
		if (mapsIt == runtimeActionMaps.end())
			return -1;

		auto nameIt = mapsIt->second.nameToIndex.find(actionMapName);
		if (nameIt == mapsIt->second.nameToIndex.end())
			return -1;

		auto& map = mapsIt->second.maps[nameIt->second];
		for (auto& action : map->GetActions())
		{
			if (action.name != actionName)
				continue;

			return action.listeners.AddHandle(listener, handler);
		}

		return -1;
	}

	void InputActionResolver::DeregisterAction(InputUserID userID, const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener)
	{
		if (!listener.IsValid())
			return;

		auto mapsIt = runtimeActionMaps.find(userID);
		if (mapsIt == runtimeActionMaps.end())
			return;

		auto nameIt = mapsIt->second.nameToIndex.find(actionMapName);
		if (nameIt == mapsIt->second.nameToIndex.end())
			return;

		auto& map = mapsIt->second.maps[nameIt->second];
		for (auto& action : map->GetActions())
		{
			if (action.name != actionName)
				continue;

			action.listeners.RemoveByListener(&listener);
		}
	}

	void InputActionResolver::DeregisterActionByID(InputUserID userID, const std::string& actionMapName, const std::string& actionName, ListenerID listenerID)
	{
		auto mapsIt = runtimeActionMaps.find(userID);
		if (mapsIt == runtimeActionMaps.end())
			return;

		auto nameIt = mapsIt->second.nameToIndex.find(actionMapName);
		if (nameIt == mapsIt->second.nameToIndex.end())
			return;

		auto& map = mapsIt->second.maps[nameIt->second];
		for (auto& action : map->GetActions())
		{
			if (action.name != actionName)
				continue;

			action.listeners.RemoveByID(listenerID);
		}
	}

	bool InputActionResolver::EnableActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);
		if (it == runtimeActionMaps.end())
			return false;

		auto nameIt = it->second.nameToIndex.find(name);
		if (nameIt == it->second.nameToIndex.end())
			return false;

		it->second.maps[nameIt->second]->Enable();

		return true;
	}

	bool InputActionResolver::DisableActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);
		if (it == runtimeActionMaps.end())
			return false;

		auto nameIt = it->second.nameToIndex.find(name);
		if (nameIt == it->second.nameToIndex.end())
			return false;

		it->second.maps[nameIt->second]->Disable();

		return true;
	}

	bool InputActionResolver::SwitchToActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);
		if (it == runtimeActionMaps.end())
			return false;

		auto nameIt = it->second.nameToIndex.find(name);
		if (nameIt == it->second.nameToIndex.end())
			return false;

		for (auto& map : it->second.maps)
		{
			map->Disable();
		}

		it->second.maps[nameIt->second]->Enable();

		return true;
	}

	std::optional<InputActionView> InputActionResolver::GetActionState(InputUserID userID, const std::string actionName)
	{
		auto mapsIt = runtimeActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end())
			return std::nullopt;

		for (const auto& map : mapsIt->second.maps)
		{
			for (const auto& action : map->GetActions())
			{
				if (action.name == actionName)
					return InputActionView(action.phase, action.value);
			}
		}

		return std::nullopt;
	}
}