#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/Rendering/TileMap/Abstractions/TileMapRenderer.h"
#include "Models/Rendering/TileEditors/Tiled/TiledMap.h"


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Engine
{
	class Transform;


	class DF2D_API TiledMapCompatibleRenderer : public TileMapRenderer<std::shared_ptr<Models::TiledMap>>
	{
		TYPE_INFO(TiledMapCompatibleRenderer, TileMapRenderer<std::shared_ptr<Models::TiledMap>>);


	private:
		std::unordered_map<int, int> tileIDToTileSet;

		ComponentHandle<Transform> transform;

		Core::TextureManager* textureManager = nullptr;


	public:
		TiledMapCompatibleRenderer(std::shared_ptr<Models::TiledMap> tileMap, bool extendMapToRenderTarget = false);


		virtual void Init() override;

		virtual void Draw() override;
	};
}