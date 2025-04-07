#include "Constants/TiledPropertyNames.h"
#include "Map/GameMap.h"
#include <Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h>
#include <Components/TileMap/Tiled/TiledMapCompatibleRenderer.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>
#include <variant>


GameMap::GameMap(const char* mapSource, bool extendMapToRenderTarget)
{
	fullTileMap = TiledMapParser().Parse(mapSource);

	tileRenderer = AddComponent<TiledMapCompatibleRenderer>(RetrieveRenderMap(), extendMapToRenderTarget);
	tileCollider = AddComponent<TiledMapCompatibleCollider2D>(RetrieveCollisionMap());
}

std::shared_ptr<TiledMap> GameMap::RetrieveRenderMap()
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

std::vector<TiledLayer> GameMap::RetrieveCollisionMap()
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