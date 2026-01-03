#include "Core/SubSystems/Systems/Rendering/Renderer.h"
#include "Core/SubSystems/Systems/TextureManager.h"
#include "Engine/Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Models;


	TiledMapCompatibleRenderer::TiledMapCompatibleRenderer(std::shared_ptr<TiledMap> tileMap, bool extendMapToRenderTarget)
	{
		this->tileMap = tileMap;

		if (!extendMapToRenderTarget)
			return;

		Renderer::SetResolutionTarget({ tileMap->width * tileMap->tileSize + 32, tileMap->height * tileMap->tileSize });
	}

	void TiledMapCompatibleRenderer::Init()
	{
		auto size = static_cast<int>(tileMap->tileSets.size());

		for (auto i = 0; i < size; i++)
		{
			for (auto id = tileMap->tileSets[i].firstID; id <= tileMap->tileSets[i].lastID; id++)
			{
				tileIDToTileSet[id] = i;
			}
		}
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

					auto src = SDL_Rect
					{ 
						.x = tileCol * tileSetSize,
						.y = tileRow * tileSetSize,
						.w = tileSetSize,
						.h = tileSetSize
					};
					auto dest = SDL_Rect
					{ 
						.x = j * tileSetSize, 
						.y = i * tileSetSize, 
						.w = tileSetSize, 
						.h = tileSetSize 
					};

					TextureManager::DrawTextureWorldSpace(tileSet.tileSetTexture, &src, &dest);
				}
			}
		}
	}
}