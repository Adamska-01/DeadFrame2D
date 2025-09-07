#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Components/Collisions/Tile/TileCollider2D.h"
#include "Engine/Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"


class RigidBody2D;
class b2Fixture;


class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
{
private:
	void DeleteFixtures();


protected:
	std::vector<TiledLayer> collisionLayers;

	std::vector<b2Fixture*> fixtures;

	Vector2I tileMapDimension;

	int tileSize;


	virtual void RebuildFixture() override;


public:
	TiledMapCompatibleCollider2D(const PhysicsMaterial& physicsMaterial = PhysicsMaterial());

	virtual ~TiledMapCompatibleCollider2D() override;


	virtual void Init() override;


	const std::vector<TiledLayer>& GetCollisionLayers() const;
	
	const Vector2I& GetTileMapDimensions() const;

	int GetTileSize() const;
};