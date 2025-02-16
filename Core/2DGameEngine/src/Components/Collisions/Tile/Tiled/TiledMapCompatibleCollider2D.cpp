#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "GameObject.h"
#include "Tools/Collisions/ICollisionVisitor.h"


TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(TiledMapDelegate collisionMapChecker)
	: collisionMapChecker(collisionMapChecker), tileMapDimension(Vector2I::Zero), tileSize(0)
{
	collisionLayers.clear();
}

void TiledMapCompatibleCollider2D::Init()
{
	TileCollider2D::Init();

	tileMapRenderer = OwningObject->GetComponent<TiledMapCompatibleRenderer>();

	if (tileMapRenderer == nullptr)
		throw std::runtime_error("Failed to get TiledMapCompatibleRenderer from OwningObject.");

	const auto& tileMap = tileMapRenderer->GetTileMap();

	tileSize = tileMap->tileSize;

	tileMapDimension = Vector2I(tileMap->width, tileMap->height);

	collisionLayers = collisionMapChecker(tileMap);
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

const int& TiledMapCompatibleCollider2D::GetTileSize() const
{
	return tileSize;
}
