#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "GameObject.h"


TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(const char* mapSource, bool extendMapToRenderTarget)
{
}

void TiledMapCompatibleCollider2D::Init()
{
	tileMapRenderer = OwningObject->GetComponent<TiledMapCompatibleRenderer>();

	if (tileMapRenderer == nullptr)
		throw std::runtime_error("Failed to get TiledMapCompatibleRenderer from OwningObject.");
}
