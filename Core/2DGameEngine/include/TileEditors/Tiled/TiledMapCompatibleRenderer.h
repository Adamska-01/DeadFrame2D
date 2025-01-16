#pragma once
#include "Models/TiledMap.h"
#include "TileEditors/TileMapRenderer.h"



class TiledMapCompatibleRenderer : public TileMapRenderer<std::shared_ptr<TiledMap>>
{
private:
	std::unordered_map<int, int> tileIDToTileSet;


public:
	TiledMapCompatibleRenderer(std::shared_ptr<TiledMap> tileMap, bool extendMapToRenderTarget = false);


	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};