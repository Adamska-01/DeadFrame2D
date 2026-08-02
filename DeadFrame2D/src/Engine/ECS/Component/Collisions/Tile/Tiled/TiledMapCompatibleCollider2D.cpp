#include "Constants/TiledPropertyNames.h"
#include "Engine/ECS/Component/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Engine/ECS/Component/Physics/RigidBody2D.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Constants;
	using namespace DF2D::Data;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	TiledMapCompatibleCollider2D::TiledMapCompatibleCollider2D(const PhysicsMaterial& physicsMaterial)
		: TileCollider2D(physicsMaterial)
	{
		fixtures.clear();
		collisionLayers.clear();

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

		for (auto fixture : fixtures)
		{
			if (fixture == 0)
				continue;

			rigidBody->DestroyFixture(fixture);
		}
	}

	void TiledMapCompatibleCollider2D::RebuildFixture()
	{
		DeleteFixtures();

		SearchRigidBody();

		if (rigidBody == nullptr)
			return;

		fixtures.clear();

		auto angle = transform->GetWorldRotation() * (MathConstants::PI_f / 180.0f);

		for (const auto& layer : collisionLayers)
		{
			for (auto i = 0; i < tileMapDimension.y; ++i)
			{
				for (auto j = 0; j < tileMapDimension.x; ++j)
				{
					auto tileID = layer.Data[i][j];

					if (tileID == 0)
						continue;

					physicsMaterial.shape = BoxShapeDefinition2D
					{
						.halfExtents = Vector2F(tileSize * 0.5f, tileSize * 0.5f),
						.center = Vector2F((j * tileSize + tileSize * 0.5f), (i * tileSize + tileSize * 0.5f)),
						.angle = angle
					};

					physicsMaterial.density = layer.GetFloatProperty(TiledPropertyNames::DENSITY, 1.0f);
					physicsMaterial.friction = layer.GetFloatProperty(TiledPropertyNames::FRICTION, 0.3f);
					physicsMaterial.isSensor = layer.GetBoolProperty(TiledPropertyNames::IS_SENSOR, false);
					physicsMaterial.restitution = layer.GetFloatProperty(TiledPropertyNames::RESTITUTION, 0.0f);
					physicsMaterial.restitutionThreshold = layer.GetFloatProperty(TiledPropertyNames::RESTITUTION_THRESHOLD, 1.0f);

					fixtures.push_back(rigidBody->CreateFixture(physicsMaterial, GetHandleAs<ContactEventProvider>()));
				}
			}
		}

		isDirty = false;
	}

	void TiledMapCompatibleCollider2D::Init()
	{
		TileCollider2D::Init();

		auto gameObject = GetGameObject();

		tileMapRenderer = Guard::AgainstNullAssignment(gameObject->GetComponent<TiledMapCompatibleRenderer>(), NAME_OF(tileMapRenderer));

		const auto& tileMap = tileMapRenderer->GetTileMap();

		tileSize = tileMap->tileSize;

		tileMapDimension = Vector2I(tileMap->width, tileMap->height);

		collisionLayers = tileMap->layers;

		MarkDirty();
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
}