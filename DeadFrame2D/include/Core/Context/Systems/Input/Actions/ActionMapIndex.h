#pragma once
#include "Core/Context/Systems/Input/Actions/RuntimeActionMap.h"
#include "DF2D_API.h"
#include <string>
#include <unordered_map>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API ActionMapIndex
	{
	public:
		std::vector<std::shared_ptr<RuntimeActionMap>> maps;

		std::unordered_map<std::string, size_t> nameToIndex;
	};
}