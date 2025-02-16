#pragma once
#include "Components/Collisions/Tile/TileCollider2D.h"
#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Math/Vector2.h"
#include "TileEditors/Tiled/Models/TiledMap.h"
#include <functional>


using TiledMapDelegate = std::function<std::vector<TiledLayer>(std::weak_ptr<TiledMap>)>;


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
private:
	std::vector<TiledLayer> collisionLayers;

	TiledMapDelegate collisionMapChecker;

	Vector2I tileMapDimension;

	int tileSize;


public:
	TiledMapCompatibleCollider2D(TiledMapDelegate collisionMapChecker);


	virtual void Init() override;


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor) override;


	const std::vector<TiledLayer>& GetCollisionLayers() const;
	
	const Vector2I& GetTileMapDimensions() const;

	const int& GetTileSize() const;
};