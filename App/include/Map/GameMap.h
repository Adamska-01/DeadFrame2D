#pragma once
#include <GameObject.h>
#include <optional>
#include <vector>


struct TiledMap;
struct TiledLayer;
struct TiledObjectGroup;
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


	std::optional<TiledObjectGroup> RetrieveObjectGroup(std::string_view groupName);
};