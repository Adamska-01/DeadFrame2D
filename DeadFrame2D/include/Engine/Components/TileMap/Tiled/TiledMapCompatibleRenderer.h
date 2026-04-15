#pragma once
#include "DF2D_API.h"
#include "Engine/Components/TileMap/TileMapRenderer.h"
#include "Models/Rendering/TileEditors/Tiled/TiledMap.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API TiledMapCompatibleRenderer : public TileMapRenderer<std::shared_ptr<Models::TiledMap>>
	{
		TYPE_INFO(TiledMapCompatibleRenderer, TileMapRenderer<std::shared_ptr<Models::TiledMap>>);


	private:
		std::unordered_map<int, int> tileIDToTileSet;


	public:
		TiledMapCompatibleRenderer(std::shared_ptr<Models::TiledMap> tileMap, bool extendMapToRenderTarget = false);


		virtual void Init() override;

		virtual void Draw() override;
	};
}