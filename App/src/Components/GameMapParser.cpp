#include "Components/GameMapParser.h"
#include <Constants/TiledPropertyNames.h>
#include <TileEditors/Tiled/Models/TiledMap.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>


GameMapParser::GameMapParser(std::string_view mapSource)
{
	fullTileMap = TiledMapParser().Parse(mapSource);
}

void GameMapParser::Init()
{
}

void GameMapParser::Update(float dt)
{
}

void GameMapParser::Draw()
{
}

std::shared_ptr<TiledMap> GameMapParser::RetrieveRenderMap()
{
	if (fullTileMap == nullptr)
		throw std::runtime_error("Trying to access tile map but Tile Renderer component is null...");

	auto renderLayer = std::vector<TiledLayer>();

	for (const auto& layer : fullTileMap->layers)
	{
		// TODO: Replace property name with a constant
		auto renderEnabled = layer.GetProperty(TiledPropertyNames::RENDER_ENABLED);

		if (!renderEnabled.has_value() || !std::get<bool>(renderEnabled.value().Value))
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

std::vector<TiledLayer> GameMapParser::RetrieveCollisionMap()
{
	if (fullTileMap == nullptr)
		throw std::runtime_error("Trying to access tile map but Tile Renderer component is null...");

	auto colliderMap = std::vector<TiledLayer>();

	for (const auto& layer : fullTileMap->layers)
	{
		// TODO: Replace property name with a constant
		auto isSolid = layer.GetProperty(TiledPropertyNames::IS_SOLID);

		if (!isSolid.has_value() || !std::get<bool>(isSolid.value().Value))
			continue;

		colliderMap.push_back(layer);
	}

	return colliderMap;
}

std::optional<TiledObjectGroup> GameMapParser::RetrieveObjectGroup(std::string_view groupName)
{
	auto it = std::find_if(fullTileMap->objectGroups.begin(), fullTileMap->objectGroups.end(),
		[&groupName](const auto& group)
		{
			return group.name == groupName;
		});

	if (it != fullTileMap->objectGroups.end())
		return *it;
	else
		return std::nullopt;
}

int GameMapParser::GetTileSize() const
{
	return fullTileMap->tileSize;
}