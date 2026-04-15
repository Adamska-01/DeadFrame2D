#pragma once
#include "TiledProperty.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


namespace DeadFrame2D::Models
{
	struct TiledLayer
	{
	private:
		template<typename T>
		T GetPropertyValue(std::string_view key, T defaultValue = {}) const
		{
			auto it = Properties.find(std::string(key));

			if (it == Properties.end())
				return defaultValue;

			const auto& prop = it->second;

			if (std::holds_alternative<T>(prop.Value))
				return std::get<T>(prop.Value);

			return defaultValue;
		}


	public:
		std::string name;

		std::unordered_map<std::string, TiledProperty> Properties;

		std::vector<std::vector<int>> Data;


		float GetFloatProperty(std::string_view key, float defaultValue = 0.0f) const
		{
			return GetPropertyValue<float>(key, defaultValue);
		}

		bool GetBoolProperty(std::string_view key, bool defaultValue = false) const
		{
			return GetPropertyValue<bool>(key, defaultValue);
		}

		int GetIntProperty(std::string_view key, int defaultValue = -1) const
		{
			return GetPropertyValue<int>(key, defaultValue);
		}

		std::string GetStringProperty(std::string_view key, std::string defaultValue = "") const
		{
			return GetPropertyValue<std::string>(key, defaultValue);
		}
	};
}