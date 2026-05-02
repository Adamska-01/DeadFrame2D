#include "Constants/Paths/ResourcePaths.h"
#include "Converters/Input/ControllerButtonConversions.h"
#include "Converters/Input/KeyboardKeyCodeConversions.h"
#include "Converters/Input/MouseButtonConversions.h"
#include "Core/Context/Systems/Input/Actions/InputActionResolver.h"
#include "Core/Context/Systems/Input/Actions/RuntimeActionMap.h"
#include "Core/Context/Systems/Input/Devices/DeviceTypes/Abstractions/InputDevice.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Context/Systems/Input/User/InputUser.h"
#include "Core/Context/Systems/Input/User/InputUserManager.h"
#include "Data/Systems/Input/InputControlState.h"
#include "Engine/ECS/System/Events/DispatchableEvent.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Input/InputUserCreatedEvent.h"
#include "Engine/Events/Context/Input/InputUserDestroyedEvent.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include "Utilities/IO/Serialization/JsonSerializer.h"


namespace DeadFrame2D::Core
{
	using namespace DeadFrame2D::Engine;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;
	using namespace DeadFrame2D::Internal;
	using namespace DeadFrame2D::Constants;
	using namespace DeadFrame2D::Models;


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

		auto sdlCode = ToSDLCode(device, controlType, simpleID);
		auto state = controlType == InputControlType::DIGITAL
			? device.GetButtonState(sdlCode)
			: device.GetAxisState(sdlCode);

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

	void InputActionResolver::InputUserCreatedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserCreatedEvent = DispatchableEvent::SafeCast<InputUserCreatedEvent>(dispatchableEvent);

		if (inputUserCreatedEvent == nullptr)
			return;

		auto userID = inputUserCreatedEvent->GetInputUserID();

		if (!JsonSerializer::IsSerializable<InputActionMapBucket>())
			throw std::runtime_error("[Input] InputActionMapBucket is not serializable. Cannot load input configuration...");

		auto actionMapBucket = JsonSerializer::DeserializeFromFile<InputActionMapBucket>(Paths::Files::INPUT_CONTROLS);

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

	void InputActionResolver::InputUserDestroyedEventHandler(std::shared_ptr<DispatchableEvent> dispatchableEvent)
	{
		auto inputUserDestroyedEvent = DispatchableEvent::SafeCast<InputUserDestroyedEvent>(dispatchableEvent);

		if (inputUserDestroyedEvent == nullptr)
			return;

		auto userID = inputUserDestroyedEvent->GetInputUserID();

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
		auto inputUser = Input::Users()->GetUserFromPairedDevice(device.ID());
		if (inputUser == nullptr)
			return;

		const auto userID = inputUser->ID();
		auto mapsIt = runtimeActionMaps.find(userID);
		if (mapsIt == runtimeActionMaps.end())
			return;

		auto& userActiveActions = activeActions[userID];
		const auto customCode = ToCustomCode(device, inputControlType, controlID);

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
						matches = std::get<int>(binding.input.value) == customCode;
						break;

					case BindingType::COMPOSITE_1D:
					{
						auto& comp = std::get<Composite1D>(binding.input.value);
						matches = comp.negative == customCode || comp.positive == customCode;
						break;
					}

					case BindingType::COMPOSITE_2D:
					{
						auto& comp = std::get<Composite2D>(binding.input.value);
						matches = comp.up == customCode || comp.down == customCode ||
								  comp.left == customCode || comp.right == customCode;
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