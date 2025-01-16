#pragma once
#include <GameObject.h>
#include <TileEditors/Tiled/Parsers/TiledMapParser.h>
#include <TileEditors/Tiled/TiledMapCompatibleRenderer.h>
#include <vector>


class GameMap : GameObject
{
private:
	TiledMapParser gameMapParser;

	// TODO: Make this a GameComponennt
	std::shared_ptr<TiledMapCompatibleRenderer> tiledMapCompatibleRenderer;


public:
	GameMap(std::string mapSource);

	~GameMap();

	
	virtual void Update(float dt) override;

	virtual void Draw() override;

	virtual void Clean() override;
};