#include "Components/GameMapParser.h"
#include <algorithm>
#include <TileEditors/Tiled/Models/TiledLayer.h>
#include <TileEditors/Tiled/Models/TiledMap.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>


GameMapParser::GameMapParser(std::string_view mapSource)
{
	fullTileMap = TiledMapParser().Parse(mapSource);
}

void GameMapParser::Init()
{

}

void GameMapParser::Start()
{

}

void GameMapParser::Update(float dt)
{

}

void GameMapParser::Draw()
{

}

std::shared_ptr<TiledMap> GameMapParser::RetrieveRenderMap(std::vector<std::string_view> layerNames)
{
	if (fullTileMap == nullptr)
		throw std::runtime_error("Trying to access tile map but Tile Renderer component is null...");

	auto renderLayer = std::vector<TiledLayer>();

	for (const auto& layer : fullTileMap->layers)
	{
		auto it = std::find(layerNames.begin(), layerNames.end(), layer.name);

		if (it == layerNames.end())
			continue;

		renderLayer.push_back(layer);
	}

	return std::make_shared<TiledMap>(
		fullTileMap->width,
		fullTileMap->height,
		fullTileMap->tileSize,
		fullTileMap->tileSets,
		renderLayer);
}

std::optional<TiledObjectGroup> GameMapParser::RetrieveObjectGroup(std::string_view groupName)
{
	auto it = std::find_if(fullTileMap->objectGroups.begin(), fullTileMap->objectGroups.end(),
		[&groupName](const auto& group)
		{
			return group.name == groupName;
		});

	if (it == fullTileMap->objectGroups.end())
		return std::nullopt;

	return *it;
}

int GameMapParser::GetTileSize() const
{
	return fullTileMap->tileSize;
}