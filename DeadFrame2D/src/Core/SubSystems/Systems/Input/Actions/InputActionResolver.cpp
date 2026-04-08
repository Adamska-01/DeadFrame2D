#include "Converters/Input/ControllerButtonConversions.h"
#include "Converters/Input/KeyboardKeyCodeConversions.h"
#include "Converters/Input/MouseButtonConversions.h"
#include "Core/SubSystems/Systems/Input/Actions/InputActionResolver.h"
#include "Core/SubSystems/Systems/Input/Actions/RuntimeActionMap.h"
#include "Core/SubSystems/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/SubSystems/Systems/Input/Input.h"
#include "Core/SubSystems/Systems/Input/User/InputUser.h"
#include "Core/SubSystems/Systems/Input/User/InputUserManager.h"
#include "Data/Input/InputControlState.h"
#include "Engine/EngineEvents/DispatchableEvent.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserCreatedEvent.h"
#include "Engine/EngineEvents/Events/SubSystems/Input/InputUserDestroyedEvent.h"
#include <Constants/ResourcePaths.h>
#include <Models/Input/ActionMap/InputActionMapBucket.h>
#include <Tools/Serialization/JsonSerializer.h>


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;
	using namespace DeadFrame2D::Internal;

	using namespace Shared::Constants;
	using namespace Shared::Models;


	InputActionResolver::InputActionResolver()
	{
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(InputUserCreatedEvent)), this, &InputActionResolver::InputUserCreatedEventHandler);
		EventDispatcher::RegisterEventHandler(std::type_index(typeid(InputUserDestroyedEvent)), this, &InputActionResolver::InputUserDestroyedEventHandler);
	}

	InputActionResolver::~InputActionResolver()
	{
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(InputUserCreatedEvent)), this);
		EventDispatcher::DeregisterEventHandler(std::type_index(typeid(InputUserDestroyedEvent)), this);
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

		auto state = binding.input.controlType == InputControlType::DIGITAL
			? device.GetButtonState(ToSDLCode(device, controlType, simpleID))
			: device.GetAxisState(ToSDLCode(device, controlType, simpleID));

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

		auto neg = binding.input.controlType == InputControlType::DIGITAL
			? device.GetButtonState(ToSDLCode(device, controlType, comp.negative))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.negative));
		auto pos = binding.input.controlType == InputControlType::DIGITAL
			? device.GetButtonState(ToSDLCode(device, controlType, comp.positive))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.positive));

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

		auto up = binding.input.controlType == InputControlType::DIGITAL 
			? device.GetButtonState(ToSDLCode(device, controlType, comp.up))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.up));
		auto down = binding.input.controlType == InputControlType::DIGITAL 
			? device.GetButtonState(ToSDLCode(device, controlType, comp.down))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.down));
		auto left = binding.input.controlType == InputControlType::DIGITAL
			? device.GetButtonState(ToSDLCode(device, controlType, comp.left))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.left));
		auto right = binding.input.controlType == InputControlType::DIGITAL
			? device.GetButtonState(ToSDLCode(device, controlType, comp.right))
			: device.GetAxisState(ToSDLCode(device, controlType, comp.right));

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

	int InputActionResolver::ToCustomCode(const InputDevice& device, InputControlType inputControlType, int sdlCode)
	{
		switch (device.Type())
		{
		case InputDeviceType::KEYBOARD:
		{
			auto scancode = static_cast<SDL_Scancode>(sdlCode);

			return static_cast<int>(KeyboardKeyCodeConversions::ToKeyboardKeyCode(scancode));
		}

		case InputDeviceType::MOUSE:
		{
			if (inputControlType == InputControlType::DIGITAL)
				return static_cast<int>(MouseButtonConversions::ToMouseButtonCode(sdlCode));

			return sdlCode;
		}

		case InputDeviceType::CONTROLLER:
		{
			if (inputControlType == InputControlType::DIGITAL)
			{
				auto sdlButtonCode = static_cast<SDL_GameControllerButton>(sdlCode);

				return static_cast<int>(ControllerButtonConversions::ToControllerButtonCode(sdlButtonCode));
			}
			else
			{
				auto sdlAxisCode = static_cast<SDL_GameControllerAxis>(sdlCode);

				return static_cast<int>(ControllerButtonConversions::ToControllerAxisCode(sdlAxisCode));
			}
		}

		default:
			return sdlCode;
		}
	}

	int InputActionResolver::ToSDLCode(const InputDevice& device, InputControlType inputControlType, int customCode)
	{
		switch (device.Type())
		{
		case InputDeviceType::KEYBOARD:
		{
			auto keyCode = static_cast<KeyboardKeyCode>(customCode);

			return static_cast<int>(KeyboardKeyCodeConversions::ToSDLScancode(keyCode));
		}

		case InputDeviceType::MOUSE:
		{
			if (inputControlType == InputControlType::DIGITAL)
			{
				auto buttonCode = static_cast<MouseButtonCode>(customCode);

				return static_cast<int>(MouseButtonConversions::ToSDLMouseButton(buttonCode));
			}

			return customCode;
		}

		case InputDeviceType::CONTROLLER:
		{
			if (inputControlType == InputControlType::DIGITAL)
			{
				auto buttonCode = static_cast<ControllerButtonCode>(customCode);

				return static_cast<int>(ControllerButtonConversions::ToSDLControllerButton(buttonCode));
			}
			else
			{
				auto axisCode = static_cast<ControllerAxisCode>(customCode);

				return static_cast<int>(ControllerButtonConversions::ToSDLControllerAxis(axisCode));
			}
		}

		default:
			return customCode;
		}
	}

	void InputActionResolver::InputUserCreatedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent)
	{
		auto inputUserCreatedEvent = DispatchableEvent::SafeCast<InputUserCreatedEvent>(dispatchableEvent);

		if (inputUserCreatedEvent == nullptr)
			return;

		auto userID = inputUserCreatedEvent->GetInputUserID();

		if (!Shared::Tools::IsSerializable<InputActionMapBucket>())
		{
			throw std::runtime_error("[Input] InputActionMapBucket is not serializable. Cannot load input configuration...");
		}

		auto actionMapBucket = Shared::Tools::DeserializeFromFile<InputActionMapBucket>(Paths::Files::INPUT_CONTROLS);

		// Create empty slots
		runtimeActionMaps[userID] = {};
		fastLookupActionMaps[userID] = {};
		activeActions[userID] = {};

		auto& runtimeMapsForBucket = runtimeActionMaps[userID];
		auto& fastLookupForBucket = fastLookupActionMaps[userID];

		// --- Construct runtime action maps ---
		for (auto& actionMap : actionMapBucket.actionMaps)
		{
			runtimeMapsForBucket.push_back(std::make_shared<RuntimeActionMap>(actionMap));
		}

		// --- Construct fast lookup map ---
		for (auto& map : runtimeMapsForBucket)
		{
			for (auto& action : map->GetActions())
			{
				for (auto& binding : action.bindings)
				{
					const auto& mapName = map->Name();
					const auto deviceType = binding.input.inputDeviceType;
					const auto controlType = binding.input.controlType;

					switch (binding.input.bindingType)
					{
						case BindingType::SIMPLE:
						{
							auto controlID = std::get<int>(binding.input.value);

							auto key = std::make_tuple(mapName, deviceType, controlType, controlID);

							fastLookupForBucket[key].push_back({ &action, &binding });
							break;
						}

						case BindingType::COMPOSITE_1D:
						{
							auto comp = std::get<Composite1D>(binding.input.value);
							
							auto keyNeg = std::make_tuple(mapName, deviceType, controlType, comp.negative);
							auto keyPos = std::make_tuple(mapName, deviceType, controlType, comp.positive);

							fastLookupForBucket[keyNeg].push_back({ &action, &binding });
							fastLookupForBucket[keyPos].push_back({ &action, &binding });
							break;
						}

						case BindingType::COMPOSITE_2D:
						{
							auto comp = std::get<Composite2D>(binding.input.value);
							
							auto keyUp = std::make_tuple(mapName, deviceType, controlType, comp.up);
							auto keyDown = std::make_tuple(mapName, deviceType, controlType, comp.down);
							auto keyLeft = std::make_tuple(mapName, deviceType, controlType, comp.left);
							auto keyRight = std::make_tuple(mapName, deviceType, controlType, comp.right);

							fastLookupForBucket[keyUp].push_back({ &action, &binding });
							fastLookupForBucket[keyDown].push_back({ &action, &binding });
							fastLookupForBucket[keyLeft].push_back({ &action, &binding });
							fastLookupForBucket[keyRight].push_back({ &action, &binding });
							break;
						}
					}
				}
			}
		}
	}

	void InputActionResolver::InputUserDestroyedEventHandler(std::shared_ptr<DeadFrame2D::Engine::DispatchableEvent> dispatchableEvent)
	{
		auto inputUserDestroyedEvent = DispatchableEvent::SafeCast<InputUserDestroyedEvent>(dispatchableEvent);

		if (inputUserDestroyedEvent == nullptr)
			return;

		auto userID = inputUserDestroyedEvent->GetInputUserID();

		runtimeActionMaps.erase(userID);
		fastLookupActionMaps.erase(userID);
		activeActions.erase(userID);
	}

	void InputActionResolver::BeginFrame()
	{
		for (auto& [userID, actionSet] : activeActions)
		{
			for (auto* action : actionSet)
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
							started = val.Magnitude() > 0.001f && !(prev.Magnitude() > 0.001f);
							held = val.Magnitude() > 0.001f && (prev.Magnitude() > 0.001f);
							cancelled = !(val.Magnitude() > 0.001f) && (prev.Magnitude() > 0.001f);
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

			action->listeners.Broadcast(*action);

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
		auto inputUser = Input::Users()->GetUserFromPairedDevice(device.ID());

		if (inputUser == nullptr)
			return;

		const auto userID = inputUser->ID();

		auto mapsIt = runtimeActionMaps.find(userID);
		auto lookupIt = fastLookupActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end() || lookupIt == fastLookupActionMaps.end())
			return;

		auto& userMaps = mapsIt->second;
		auto& userLookup = lookupIt->second;
		auto& userActiveActions = activeActions[userID];

		for (auto& map : userMaps)
		{
			if (!map->IsEnabled())
				continue;

			auto key = std::make_tuple(map->Name(), device.Type(), inputControlType, ToCustomCode(device, inputControlType, controlID));
			auto it = userLookup.find(key);

			if (it == userLookup.end())
				continue;

			for (auto& record : it->second)
			{
				userActiveActions.insert(record.action);

				switch (record.binding->input.bindingType)
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

	ListenerID InputActionResolver::RegisterAction(InputUserID userID, const std::string& actionMapName, const std::string& actionName, const ComponentHandleBase& listener, const std::function<void(const RuntimeInputAction&)>& handler)
	{
		if (!listener.IsValid())
			return -1;

		auto mapsIt = runtimeActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end())
			return -1;

		for (const auto& map : mapsIt->second)
		{
			if (map->Name() != actionMapName)
				continue;

			for (auto& action : map->GetActions())
			{
				if (action.name != actionName)
					continue;

				return action.listeners.AddHandle(listener, handler);
			}
		}

		return -1;
	}

	void InputActionResolver::DeregisterAction(InputUserID userID, const std::string& actionMapName, const std::string& actionName, const DeadFrame2D::Engine::ComponentHandleBase& listener)
	{
		if (!listener.IsValid())
			return;

		auto mapsIt = runtimeActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end())
			return;

		for (const auto& map : mapsIt->second)
		{
			if (map->Name() != actionMapName)
				continue;

			for (auto& action : map->GetActions())
			{
				if (action.name != actionName)
					continue;

				action.listeners.RemoveByListener(&listener);
			}
		}
	}

	void InputActionResolver::DeregisterActionByID(InputUserID userID, const std::string& actionMapName, const std::string& actionName, DeadFrame2D::Utilities::ListenerID listenerID)
	{
		auto mapsIt = runtimeActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end())
			return;

		for (const auto& map : mapsIt->second)
		{
			if (map->Name() != actionMapName)
				continue;

			for (auto& action : map->GetActions())
			{
				if (action.name != actionName)
					continue;

				action.listeners.RemoveByID(listenerID);
			}
		}
	}

	bool InputActionResolver::EnableActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);

		if (it == runtimeActionMaps.end())
			return false;

		for (auto& map : it->second)
		{
			if (map->Name() != name)
				continue;

			map->Enable();

			return true;
		}

		return false;
	}

	bool InputActionResolver::DisableActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);

		if (it == runtimeActionMaps.end())
			return false;

		for (auto& map : it->second)
		{
			if (map->Name() != name)
				continue;

			map->Disable();

			return true;
		}

		return false;
	}

	bool InputActionResolver::SwitchToActionMap(InputUserID userID, const std::string& name)
	{
		auto it = runtimeActionMaps.find(userID);

		if (it == runtimeActionMaps.end())
			return false;

		auto switched = false;

		for (auto& map : it->second)
		{
			if (map->Name() == name)
			{
				map->Enable();

				switched = true;
			}
			else
			{
				map->Disable();
			}
		}

		return switched;
	}

	std::optional<RuntimeInputAction> InputActionResolver::GetActionState(InputUserID userID, const std::string actionName)
	{
		auto mapsIt = runtimeActionMaps.find(userID);

		if (mapsIt == runtimeActionMaps.end())
			return std::nullopt;

		for (const auto& map : mapsIt->second)
		{
			for (const auto& action : map->GetActions())
			{
				if (action.name == actionName)
					return action;
			}
		}

		return std::nullopt;
	}
}