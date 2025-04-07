#pragma once
#include <GameObject.h>
#include <vector>


struct TiledMap;
struct TiledLayer;
class TiledMapCompatibleRenderer;
class TiledMapCompatibleCollider2D;


class GameMap : public GameObject
{
private:
	std::shared_ptr<TiledMap> fullTileMap;

	TiledMapCompatibleRenderer* tileRenderer;

	TiledMapCompatibleCollider2D* tileCollider;


	std::shared_ptr<TiledMap> RetrieveRenderMap();

	std::vector<TiledLayer> RetrieveCollisionMap();


public:
	GameMap(const char* mapSource, bool extendMapToRenderTarget = false);

	virtual ~GameMap() = default;
};