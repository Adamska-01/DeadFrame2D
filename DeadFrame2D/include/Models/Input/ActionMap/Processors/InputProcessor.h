#pragma once
#include "Models/Input/ActionMap/Processors/Settings/ClampProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/DeadzoneProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/InvertProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/NormalizeProcessorSettings.h"
#include "Models/Input/ActionMap/Processors/Settings/ScaleProcessorSettings.h"
#include "Models/Input/ActionMap/Types/ProcessorType.h"
#include <stdexcept>
#include <variant>


namespace DF2D::Models
{
	struct InputProcessor
	{
		ProcessorType kind;

		std::variant<
			ClampProcessorSettings,
			DeadzoneProcessorSettings,
			InvertProcessorSettings,
			NormalizeProcessorSettings,
			ScaleProcessorSettings> settings;
	};


	inline void to_json(nlohmann::json& j, const InputProcessor& p)
	{
		j["kind"] = p.kind;

		std::visit(
			[&](auto&& arg)
			{
				using T = std::decay_t<decltype(arg)>;
				if constexpr (std::is_same_v<T, ClampProcessorSettings>)
				{
					j["settings"] = { { "min", arg.min }, { "max", arg.max } };
				}
				else if constexpr (std::is_same_v<T, DeadzoneProcessorSettings>)
				{
					j["settings"] = { { "threshold", arg.threshold } };
				}
				else if constexpr (std::is_same_v<T, ScaleProcessorSettings>)
				{
					j["settings"] = { { "factor", arg.factor } };
				}
			},
			p.settings);
	}

	inline void from_json(const nlohmann::json& j, InputProcessor& p)
	{
		p.kind = j.at("kind").get<ProcessorType>();

		switch (p.kind)
		{
		case ProcessorType::DEADZONE:
		{
			const auto& settingsObj = j.at("settings");

			p.settings = DeadzoneProcessorSettings
			{
				.threshold = settingsObj.at("threshold").get<float>()
			};
			break;
		}

		case ProcessorType::SCALE:
		{
			const auto& settingsObj = j.at("settings");

			p.settings = ScaleProcessorSettings
			{
				.factor = settingsObj.at("factor").get<float>()
			};
			break;
		}

		case ProcessorType::INVERT:
			p.settings = InvertProcessorSettings();
			break;

		case ProcessorType::NORMALIZE:
			p.settings = NormalizeProcessorSettings{};
			break;

		case ProcessorType::CLAMP:
		{
			const auto& settingsObj = j.at("settings");

			p.settings = ClampProcessorSettings
			{
				.min = settingsObj.at("min").get<float>(),
				.max = settingsObj.at("max").get<float>()
			};
			break;
		}

		default:
			throw std::runtime_error("Unknown ProcessorType in InputProcessor");
		}
	}
}