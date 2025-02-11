#pragma once
#include "Components/Collisions/Tile/TileCollider2D.h"
#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "TileEditors/Tiled/Models/TiledMap.h"
#include <functional>


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
private:
	std::weak_ptr<TiledMap> collisionMap;

	std::function<std::weak_ptr<TiledMap>(std::weak_ptr<TiledMap>)> collisionMapChecker;


public:
	TiledMapCompatibleCollider2D(std::function<std::weak_ptr<TiledMap>(std::weak_ptr<TiledMap>)> collisionMapChecker);


	virtual void Init() override;


	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) override;
};