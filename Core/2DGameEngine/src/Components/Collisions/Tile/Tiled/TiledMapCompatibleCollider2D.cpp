#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "GameObject.h"
#include "Tools/Collisions/ICollisionVisitor.h"


TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(std::vector<TiledLayer> collisionLayers)
	: collisionLayers(collisionLayers)
{
	tileSize = 0;
	tileMapDimension = Vector2I::Zero;
}

void TiledMapCompatibleCollider2D::Init()
{
	TileCollider2D::Init();

	tileMapRenderer = OwningObject->GetComponent<TiledMapCompatibleRenderer>();

	// Shouldn't have a tile colliderr without a tile renderer
	if (tileMapRenderer == nullptr)
		throw std::runtime_error("Failed to get TiledMapCompatibleRenderer from OwningObject.");

	const auto& tileMap = tileMapRenderer->GetTileMap();

	tileSize = tileMap->tileSize;

	tileMapDimension = Vector2I(tileMap->width, tileMap->height);
}

bool TiledMapCompatibleCollider2D::Accept(ICollisionVisitor& visitor, Collider2D* other)
{
	return other->AcceptDispatch(this, visitor);
}

bool TiledMapCompatibleCollider2D::AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

bool TiledMapCompatibleCollider2D::AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor)
{
	return visitor.Visit(this, other);
}

const std::vector<TiledLayer>& TiledMapCompatibleCollider2D::GetCollisionLayers() const
{
	return collisionLayers;
}

const Vector2I& TiledMapCompatibleCollider2D::GetTileMapDimensions() const
{
	return tileMapDimension;
}

int TiledMapCompatibleCollider2D::GetTileSize() const
{
	return tileSize;
}