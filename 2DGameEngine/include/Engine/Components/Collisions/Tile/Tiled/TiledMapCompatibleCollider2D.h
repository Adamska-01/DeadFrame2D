#pragma once
#include "Core/Math/Vector2.h"
#include "Engine/Components/Collisions/Tile/TileCollider2D.h"
#include "Engine/Components/TileMap/Tiled/TiledMapCompatibleRenderer.h"


class b2Fixture;


namespace DeadFrame2D::Engine
{
	class TiledMapCompatibleCollider2D : public TileCollider2D<TiledMapCompatibleRenderer>
	{
	private:
		void DeleteFixtures();


	protected:
		std::vector<DeadFrame2D::Models::TiledLayer> collisionLayers;

		std::vector<b2Fixture*> fixtures;

		DeadFrame2D::Core::Vector2I tileMapDimension;

		int tileSize;


		virtual void RebuildFixture() override;


	public:
		TiledMapCompatibleCollider2D(const DeadFrame2D::Data::PhysicsMaterial& physicsMaterial = DeadFrame2D::Data::PhysicsMaterial());

		virtual ~TiledMapCompatibleCollider2D() override;


		virtual void Init() override;


		const std::vector<DeadFrame2D::Models::TiledLayer>& GetCollisionLayers() const;
	
		const DeadFrame2D::Core::Vector2I& GetTileMapDimensions() const;

		int GetTileSize() const;
	};
}