#pragma once
#include "DF2D_API.h"
#include "Engine/Components/TileMap/TileMapRenderer.h"
#include "Models/TileEditors/Tiled/TiledMap.h"


namespace DeadFrame2D::Engine
{
	class DF2D_API TiledMapCompatibleRenderer : public TileMapRenderer<std::shared_ptr<DeadFrame2D::Models::TiledMap>>
	{
	private:
		std::unordered_map<int, int> tileIDToTileSet;


	public:
		TiledMapCompatibleRenderer(std::shared_ptr<DeadFrame2D::Models::TiledMap> tileMap, bool extendMapToRenderTarget = false);


		virtual void Init() override;

		virtual void Start() override;

		virtual void Update(float dt) override;

		virtual void Draw() override;
	};
}