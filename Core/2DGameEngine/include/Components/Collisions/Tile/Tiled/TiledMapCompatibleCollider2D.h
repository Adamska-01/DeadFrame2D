#pragma once
#include "Components/Collisions/Tile/TileCollider2D.h"
#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
public:
	TiledMapCompatibleCollider2D(const char* mapSource, bool extendMapToRenderTarget = false);


	virtual void Init() override;
};