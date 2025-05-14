#pragma once
#include "Components/Collisions/Tile/TileCollider2D.h"
#include "Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"
#include "Math/Vector2.h"


class RigidBody2D;
class b2Fixture;


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
protected:
	std::vector<TiledLayer> collisionLayers;

	std::vector<b2Fixture*> fixtures;

	Vector2I tileMapDimension;

	int tileSize;


	virtual void RebuildFixture() override;


public:
	TiledMapCompatibleCollider2D(std::vector<TiledLayer> collisionLayers, const PhysicsMaterial& physicsMaterial = PhysicsMaterial());

	virtual ~TiledMapCompatibleCollider2D() override;


	virtual void Init() override;


	const std::vector<TiledLayer>& GetCollisionLayers() const;
	
	const Vector2I& GetTileMapDimensions() const;

	int GetTileSize() const;
};