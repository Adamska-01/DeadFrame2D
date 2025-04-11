#pragma once
#include <vector>
#include <nlohmann/json.hpp>


class BobbleGridLevel 
{
public:
	int width;
	
	int height;

	std::vector<int> grid;
};


inline void to_json(nlohmann::json& j, const BobbleGridLevel& grid)
{
	j = nlohmann::json
	{
		{ "width", grid.width },
		{ "height", grid.height },
		{ "grid", grid.grid }
	};
};

inline void from_json(const nlohmann::json& j, BobbleGridLevel& grid)
{
	j.at("width").get_to(grid.width);
	j.at("height").get_to(grid.height);
	j.at("grid").get_to(grid.grid);
}