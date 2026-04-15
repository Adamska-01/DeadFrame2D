#pragma once
#include "Models/Input/ActionMap/Bindings/Types/Composite1D.h"
#include "Models/Input/ActionMap/Bindings/Types/Composite2D.h"
#include "Models/Input/ActionMap/Types/BindingType.h"
#include "Models/Input/ActionMap/Types/InputControlType.h"
#include "Models/Input/Controls/ControllerAxisCode.h"
#include "Models/Input/Controls/ControllerButtonCode.h"
#include "Models/Input/Controls/KeyboardKeyCode.h"
#include "Models/Input/Controls/MouseAxisCode.h"
#include "Models/Input/Controls/MouseButtonCode.h"
#include "Models/Input/InputDeviceType.h"
#include <variant>


namespace DeadFrame2D::Models
{
	struct InputBinding
	{
		InputDeviceType inputDeviceType;

		BindingType bindingType = BindingType::SIMPLE;

		InputControlType controlType;

		std::variant<
			int,
			Composite1D,
			Composite2D> value;
	};


	// Serialization functions (nlohmann)
	inline void to_json(nlohmann::json& j, const InputBinding& inputBinding)
	{
		j["inputDeviceType"] = inputBinding.inputDeviceType;
		j["bindingType"] = inputBinding.bindingType;
		j["controlType"] = inputBinding.controlType;

		auto toJsonString = [&](int v) -> nlohmann::json
		{
			switch (inputBinding.inputDeviceType)
			{
			case InputDeviceType::KEYBOARD:
				return static_cast<KeyboardKeyCode>(v);

			case InputDeviceType::MOUSE:
				if (inputBinding.controlType == InputControlType::DIGITAL)
					return static_cast<MouseButtonCode>(v);
				else // ANALOG
					return static_cast<MouseAxisCode>(v);

			case InputDeviceType::CONTROLLER:
				if (inputBinding.controlType == InputControlType::DIGITAL)
					return static_cast<ControllerButtonCode>(v);
				else // ANALOG
					return static_cast<ControllerAxisCode>(v);

			default:
				return v;
			}
		};

		std::visit(
			[&](auto&& v)
			{
				using T = std::decay_t<decltype(v)>;

				if constexpr (std::is_same_v<T, int>)
				{
					j["value"] = toJsonString(v);
				}
				else if constexpr (std::is_same_v<T, Composite1D>)
				{
					j["value"] =
					{
						{ "negative", toJsonString(v.negative) },
						{ "positive", toJsonString(v.positive) }
					};
				}
				else if constexpr (std::is_same_v<T, Composite2D>)
				{
					j["value"] =
					{
						{ "up",    toJsonString(v.up) },
						{ "down",  toJsonString(v.down) },
						{ "left",  toJsonString(v.left) },
						{ "right", toJsonString(v.right) }
					};
				}
			}, 
			inputBinding.value);
	}

	inline void from_json(const nlohmann::json& j, InputBinding& inputBinding)
	{
		// Basic fields
		j.at("inputDeviceType").get_to(inputBinding.inputDeviceType);
		j.at("bindingType").get_to(inputBinding.bindingType);
		j.at("controlType").get_to(inputBinding.controlType);

		if (!j.contains("value"))
			throw std::runtime_error("InputBinding JSON must contain 'value'");

		auto parseJsonString = [&](const nlohmann::json& js) -> int
		{
			switch (inputBinding.inputDeviceType)
			{
			case InputDeviceType::KEYBOARD:
				return static_cast<int>(js.get<KeyboardKeyCode>());

			case InputDeviceType::MOUSE:
				if (inputBinding.controlType == InputControlType::DIGITAL)
					return static_cast<int>(js.get<MouseButtonCode>());
				else
					return static_cast<int>(js.get<MouseAxisCode>());

			case InputDeviceType::CONTROLLER:
				if (inputBinding.controlType == InputControlType::DIGITAL)
					return static_cast<int>(js.get<ControllerButtonCode>());
				else
					return static_cast<int>(js.get<ControllerAxisCode>());

			default:
				throw std::runtime_error("Unknown device type in InputBinding");
			}
		};

		// Handle each binding type
		switch (inputBinding.bindingType)
		{
		case BindingType::SIMPLE:
			inputBinding.value = parseJsonString(j.at("value"));
			break;

		case BindingType::COMPOSITE_1D:
		{
			const auto& obj = j.at("value");

			inputBinding.value = Composite1D
			{
				.negative = parseJsonString(obj.at("negative")),
				.positive = parseJsonString(obj.at("positive"))
			};
			break;
		}

		case BindingType::COMPOSITE_2D:
		{
			const auto& obj = j.at("value");

			inputBinding.value = Composite2D
			{
				.up = parseJsonString(obj.at("up")),
				.down = parseJsonString(obj.at("down")),
				.left = parseJsonString(obj.at("left")),
				.right = parseJsonString(obj.at("right"))
			};
			break;
		}

		default:
			throw std::runtime_error("Unknown bindingType in InputBinding");
		}
	}
}