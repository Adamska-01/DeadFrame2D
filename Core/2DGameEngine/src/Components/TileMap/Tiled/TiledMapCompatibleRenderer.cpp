#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "SubSystems/Renderer.h"
#include "SubSystems/TextureManager.h"


TiledMapCompatibleRenderer::TiledMapCompatibleRenderer(const char* mapSource, bool extendMapToRenderTarget)
{
	this->tileMap = gameMapParser.Parse(mapSource);

	if (!extendMapToRenderTarget)
		return;

	Renderer::SetResolutionTarget({ tileMap->width * tileMap->tileSize, tileMap->height * tileMap->tileSize });
}

void TiledMapCompatibleRenderer::Init()
{
	for (auto i = 0; i < tileMap->tileSets.size(); i++)
	{
		for (auto id = tileMap->tileSets[i].firstID; id <= tileMap->tileSets[i].lastID; id++)
		{
			tileIDToTileSet[id] = i;
		}
	}
}

void TiledMapCompatibleRenderer::Update(float dt)
{
}

void TiledMapCompatibleRenderer::Draw()
{
	const auto& tileSets = tileMap->tileSets;
	

	for (const auto& layer : tileMap->layers)
	{
		for (auto i = 0; i < tileMap->height; ++i)
		{
			for (auto j = 0; j < tileMap->width; ++j)
			{
				auto tileID = layer.Data[i][j];

				// Skip empty tiles
				if (tileID == 0) 
					continue; 

				// Get tileSetIndex directly from the precomputed map
				auto tileSetIndex = tileIDToTileSet[tileID];

				// Get the corresponding TileSet
				const auto& tileSet = tileSets[tileSetIndex];
				auto tileSetSize = tileSet.tileSize;
				auto tileSetColumnCount = tileSet.columnCount;

				// Calculate tileRow and tileCol
				auto tileRow = (tileID - tileSet.firstID) / tileSetColumnCount;
				auto tileCol = (tileID - tileSet.firstID) % tileSetColumnCount;

				// Source and destination rects
				SDL_Rect src{ tileCol * tileSetSize, tileRow * tileSetSize, tileSetSize, tileSetSize };
				SDL_Rect dest{ j * tileSetSize, i * tileSetSize, tileSetSize, tileSetSize };

				TextureManager::DrawTile(tileSet.tileSetTexture.get(), src, dest);
			}
		}
	}
}

void TiledMapCompatibleRenderer::Clean()
{
}