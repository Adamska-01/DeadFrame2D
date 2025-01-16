#pragma once
#include "Components/TileMap/TileMapRenderer.h"
#include "TileEditors/Tiled/Models/TiledMap.h"
#include "TileEditors/Tiled/Parsers/TiledMapParser.h"



class TiledMapCompatibleRenderer : public TileMapRenderer<std::shared_ptr<TiledMap>>
{
private:
	TiledMapParser gameMapParser;

	std::unordered_map<int, int> tileIDToTileSet;


public:
	TiledMapCompatibleRenderer(const char* mapSource, bool extendMapToRenderTarget = false);


	virtual void Init() override;

	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};