#pragma once
#include "Components/Collisions/Tile/TileCollider2D.h"
#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Math/Vector2.h"


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
private:
	std::vector<TiledLayer> collisionLayers;

	Vector2I tileMapDimension;

	int tileSize;


public:
	TiledMapCompatibleCollider2D(std::vector<TiledLayer> collisionLayers);


	virtual void Init() override;


	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) override;

	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor) override;

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor) override;


	const std::vector<TiledLayer>& GetCollisionLayers() const;
	
	const Vector2I& GetTileMapDimensions() const;

	int GetTileSize() const;
};