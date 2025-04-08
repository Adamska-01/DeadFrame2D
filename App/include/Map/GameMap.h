#pragma once
#include <GameObject.h>


class GameMapParser;
class TiledMapCompatibleRenderer;
class TiledMapCompatibleCollider2D;


class GameMap : public GameObject
{
private:
	GameMapParser* gameMapParser;

	TiledMapCompatibleRenderer* tileRenderer;

	TiledMapCompatibleCollider2D* tileCollider;


public:
	GameMap(const char* mapSource, bool extendMapToRenderTarget = false);

	virtual ~GameMap() = default;
};