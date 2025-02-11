#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "GameObject.h"
#include "Tools/Collisions/ColliderVisitor.h"


TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(std::function<std::weak_ptr<TiledMap>(std::weak_ptr<TiledMap>)> collisionMapChecker)
	: collisionMapChecker(collisionMapChecker)
{
}

void TiledMapCompatibleCollider2D::Init()
{
	TileCollider2D::Init();

	tileMapRenderer = OwningObject->GetComponent<TiledMapCompatibleRenderer>();

	if (tileMapRenderer == nullptr)
		throw std::runtime_error("Failed to get TiledMapCompatibleRenderer from OwningObject.");

	collisionMap = collisionMapChecker(tileMapRenderer->GetTileMap());
}

bool TiledMapCompatibleCollider2D::Accept(ColliderVisitor& visitor, Collider2D& other)
{
	return visitor.Visit(*this, other);
}
