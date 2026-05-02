#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Engine/ECS/Component/Rendering/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Engine/ECS/Component/Transform.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	TiledMapCompatibleRenderer::TiledMapCompatibleRenderer(std::shared_ptr<TiledMap> tileMap, bool extendMapToRenderTarget)
	{
		this->tileMap = tileMap;

		if (!extendMapToRenderTarget)
			return;

		Renderer::SetResolutionTarget({ tileMap->width * tileMap->tileSize + 32, tileMap->height * tileMap->tileSize });
	}

	void TiledMapCompatibleRenderer::Init()
	{
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		auto size = static_cast<int>(tileMap->tileSets.size());

		for (auto i = 0; i < size; i++)
		{
			for (auto id = tileMap->tileSets[i].firstID; id <= tileMap->tileSets[i].lastID; id++)
			{
				tileIDToTileSet[id] = i;
			}
		}
	}

	// TODO: Should perform pre-culling here (Culling is performed during the rendering pipeline,
	// but iterating through every single tile every frame is not ideal. Use the camera to 
	// calculate which tiles need to be renderered (Take multiple-camera scenarios into account)
	void TiledMapCompatibleRenderer::Draw()
	{
		const auto& tileSets = tileMap->tileSets;
		
		auto rotation = transform->GetWorldRotation();

		auto batchData = SpriteBatchRenderData();
		batchData.spriteBatch.reserve(tileMap->width * tileMap->height);

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
					auto dest = SDL_FRect
					{ 
						.x = static_cast<float>(j * tileSetSize), 
						.y = static_cast<float>(i * tileSetSize), 
						.w = static_cast<float>(tileSetSize), 
						.h = static_cast<float>(tileSetSize) 
					};

					auto renderData = SpriteRenderData
					{
						.texture = tileSet.tileSetTexture.get(),
						.srcRect = src,
						.destRect = dest,
						.rotation = rotation
					};

					batchData.spriteBatch.push_back(renderData);
				}
			}
		}

		renderTask.renderData = std::move(batchData);

		RenderSystem::Submit(renderTask);
	}
}