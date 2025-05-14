#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Components/Physics/RigidBody2D.h"
#include "Components/Transform.h"
#include "GameObject.h"
#include "Tools/Helpers/Physics/PhysicsConversion.h"
#include "Tools/Helpers/Physics/PhysicsShapeCreators.h"


TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(std::vector<TiledLayer> collisionLayers, const PhysicsMaterial& physicsMaterial)
	: TileCollider2D(physicsMaterial), collisionLayers(collisionLayers)
{
	fixtures.clear();

	tileSize = 0;
	tileMapDimension = Vector2I::Zero;
}

TiledMapCompatibleCollider2D::~TiledMapCompatibleCollider2D()
{
	DeleteFixtures();
}

void TiledMapCompatibleCollider2D::DeleteFixtures()
{
	if (fixtures.size() <= 0 || rigidBody == nullptr)
		return;

	for (auto fix : fixtures)
	{
		if (fix == nullptr)
			continue;

		rigidBody->DestroyFixture(fix);
	}
}

void TiledMapCompatibleCollider2D::RebuildFixture()
{
	DeleteFixtures();

	SearchRigidBody();

	if (rigidBody == nullptr)
		return;

	fixtures.clear();

	auto angle = transform->GetWorldRotation() * (MathConstants::PI / 180.0f);

	for (auto layer : collisionLayers)
	{
		for (auto i = 0; i < tileMapDimension.y; ++i)
		{
			for (auto j = 0; j < tileMapDimension.x; ++j)
			{
				auto tileID = layer.Data[i][j];

				if (tileID == 0)
					continue;
				
				this->physicsMaterial.shape = PhysicsShapeCreators::CreateBoxShape(
					tileSize * 0.5f,
					tileSize * 0.5f,
					Vector2F((j * tileSize + tileSize * 0.5f), (i * tileSize + tileSize * 0.5f)),
					angle);

				auto def = PhysicsConversion::ToB2FixtureDef(physicsMaterial, reinterpret_cast<uintptr_t>(this));

				fixtures.push_back(rigidBody->CreateFixture(&def));

				// Clean up before creating another shape
				delete this->physicsMaterial.shape;
				this->physicsMaterial.shape = nullptr;
			}
		}
	}

	isDirty = false;
}

void TiledMapCompatibleCollider2D::Init()
{
	TileCollider2D::Init();

	tileMapRenderer = OwningObject.lock()->GetComponent<TiledMapCompatibleRenderer>();

	// Shouldn't have a tile collider without a tile renderer
	if (tileMapRenderer == nullptr)
		throw std::runtime_error("Failed to get TiledMapCompatibleRenderer from OwningObject.");

	const auto& tileMap = tileMapRenderer->GetTileMap();

	tileSize = tileMap->tileSize;

	tileMapDimension = Vector2I(tileMap->width, tileMap->height);
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