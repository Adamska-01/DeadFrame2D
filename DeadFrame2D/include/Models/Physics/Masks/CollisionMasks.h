#pragma once
#include "Models/Physics/Masks/NamedBitflag.h"
#include <nlohmann/json.hpp>
#include <vector>


namespace DF2D::Models
{
	struct CollisionMasks
	{
		std::vector<NamedBitflag> categories;


		/**
		 * @brief Get the value of a category bitflag by its name.
		 * @param name The name of the category.
		 * @return Optional 16-bit value of the bitflag if found, std::nullopt otherwise.
		 */
		inline uint16_t GetMaskFlagByName(const std::string& name) const
		{
			for (const auto& category : categories)
			{
				if (category.name == name)
					return category.value;
			}

			std::cerr << "[Error] Collision category not found: \"" << name << "\"\n";

			return 0;
		}


		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionMasks, categories)
	};
}