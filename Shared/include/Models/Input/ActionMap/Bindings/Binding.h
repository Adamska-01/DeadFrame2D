#pragma once
#include "Models/Input/ActionMap/Bindings/Types/Composite1D.h"
#include "Models/Input/ActionMap/Bindings/Types/Composite2D.h"
#include "Models/Input/ActionMap/Properties/InputInteraction.h"
#include "Models/Input/ActionMap/Properties/InputProcessor.h"
#include "Models/Input/ActionMap/Types/BindingType.h"
#include "Models/Input/InputDeviceType.h"
#include <nlohmann/json.hpp>
#include <optional>
#include <vector>


namespace Shared::Models
{
	struct Binding
	{
		std::string name;

		std::string action;

		InputDeviceType device;

		BindingType type = BindingType::SIMPLE;

		bool modifiersMustBePressedFirst = false;


		std::optional<int> control;

		std::optional<int> axisX;

		std::optional<int> axisY;

		std::optional<Composite2D> composite2D;

		std::optional<Composite1D> composite1D;


		// TODO: remove modifiers and interactions (make processors work)
		std::vector<int> modifiers;

		std::vector<InputProcessor> processors;

		std::vector<InputInteraction> interactions;
	};


	inline void to_json(nlohmann::json& j, const Binding& b)
	{
		j["name"] = b.name;
		j["action"] = b.action;
		j["device"] = b.device;
		j["type"] = b.type;
		j["modifiersMustBePressedFirst"] = b.modifiersMustBePressedFirst;

		if (b.control)
		{
			j["control"] = *b.control;
		}
		if (b.axisX)
		{
			j["axisX"] = *b.axisX;
		}
		if (b.axisY)
		{
			j["axisY"] = *b.axisY;
		}
		if (b.composite2D)
		{
			j["composite2D"] = *b.composite2D;
		}
		if (b.composite1D)
		{
			j["composite1D"] = *b.composite1D;
		}
		if (!b.modifiers.empty())
		{
			j["modifiers"] = b.modifiers;
		}
		if (!b.processors.empty())
		{
			j["processors"] = b.processors;
		}
		if (!b.interactions.empty())
		{
			j["interactions"] = b.interactions;
		}
	}

	inline void from_json(const nlohmann::json& j, Binding& b)
	{
		j.at("action").get_to(b.action);

		if (j.contains("device"))
		{
			j.at("device").get_to(b.device);
		}
		if (j.contains("type"))
		{
			j.at("type").get_to(b.type);
		}
		if (j.contains("control"))
		{
			b.control = j.at("control").get<int>();
		}
		if (j.contains("axisX"))
		{
			b.axisX = j.at("axisX").get<int>();
		}
		if (j.contains("axisY"))
		{
			b.axisY = j.at("axisY").get<int>();
		}
		if (j.contains("composite2D"))
		{
			b.composite2D = j.at("composite2D").get<Composite2D>();
		}
		if (j.contains("composite1D"))
		{
			b.composite1D = j.at("composite1D").get<Composite1D>();
		}
		if (j.contains("modifiers"))
		{
			b.modifiers = j.at("modifiers").get<std::vector<int>>();
		}
		if (j.contains("modifiersMustBePressedFirst"))
		{
			b.modifiersMustBePressedFirst = j.at("modifiersMustBePressedFirst").get<bool>();
		}
		if (j.contains("processors"))
		{
			b.processors = j.at("processors").get<std::vector<InputProcessor>>();
		}
		if (j.contains("interactions"))
		{
			b.interactions = j.at("interactions").get<std::vector<InputInteraction>>();
		}
		if (j.contains("name"))
		{
			b.name = j.at("name").get<std::string>();
		}
	}
}
