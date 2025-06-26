#include "Components/BobbleGrid.h"
#include "Components/GridCeiling.h"
#include <Components/Collisions/BoxCollider2D.h>
#include <Components/Collisions/Collider2D.h>
#include <Components/Transform.h>
#include <Constants/AssetPaths.h>
#include <GameObject.h>
#include <SubSystems/TextureManager.h>
#include <Tools/Helpers/EventHelpers.h>
#include <Tools/Helpers/Guards.h>


GridCeiling::GridCeiling()
	: ceilingLevel(10),
	ceilingTileSize(32),
	ceilingWidth(10),
	collider(nullptr),
	bobbleGrid(nullptr)
{
	spriteTexture = TextureManager::LoadTexture(AssetPaths::Sprites::MAP_TILES_PATH);

	SetCeilingTileSize(ceilingTileSize);
}

GridCeiling::~GridCeiling()
{
	if (collider == nullptr)
		return;

	// TODO: Fix this! Make GameComponents smart pointers (weak)
	//collider->DeregisterCollisionEnterHandler(reinterpret_cast<uintptr_t>(this));
}

void GridCeiling::OnCeilingCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	bobbleGrid->OnCeilingCollisionEnterHandler(collisionInfo);
}

void GridCeiling::Init()
{
	bobbleGrid = OwningObject.lock()->GetComponentInParent<BobbleGrid>();
	collider = OwningObject.lock()->GetComponent<BoxCollider2D>();

	Tools::Helpers::GuardAgainstNull(bobbleGrid, "GridCeiling::Init: BobbleGrid not found from GridCeiling.");
	Tools::Helpers::GuardAgainstNull(collider, "GridCeiling::Init: BoxCollider2D not found from GridCeiling.");

	collider->RegisterContactEnterHandler(EventHelpers::BindFunction(this, &GridCeiling::OnCeilingCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
}

void GridCeiling::Start()
{
	ceilingWidth = bobbleGrid->GetCurrentLevel().width;
	
	auto currentPos = OwningObject.lock()->GetTransform()->GetWorldPosition();

	collider->SetNewBoxShape(
		Vector2F(ceilingTileSize * ceilingWidth, ceilingTileSize),
		Vector2F((ceilingTileSize * ceilingWidth) / 2, (-ceilingTileSize / 2) + (ceilingTileSize * ceilingLevel)),
		0.0f);
}

void GridCeiling::Update(float deltaTime)
{

}

void GridCeiling::Draw()
{
	auto currentPos = OwningObject.lock()->GetTransform()->GetWorldPosition();

	for (auto i = 0; i < ceilingLevel; i++)
	{
		for (auto j = 0; j < ceilingWidth; j++)
		{
			auto dest = SDL_Rect
			{
				.x = static_cast<int>(currentPos.x + j * 32),
				.y = static_cast<int>(currentPos.y + i * 32),
				.w = 32,
				.h = 32
			};

			TextureManager::DrawTexture(
				spriteTexture, 
				i == ceilingLevel - 1 ? &srcCealingTileTop : &srcCealingTileBottom,
				&dest,
				180.0f);
		}
	}
}

void GridCeiling::SetCeilingTileSize(int ceilingTileSize)
{
	this->ceilingTileSize = ceilingTileSize;

	srcCealingTileTop = SDL_Rect
	{
		.x = 2 * ceilingTileSize,
		.y = 3 * ceilingTileSize,
		.w = ceilingTileSize,
		.h = ceilingTileSize
	};

	srcCealingTileBottom = SDL_Rect
	{
		.x = 2 * ceilingTileSize,
		.y = 1 * ceilingTileSize,
		.w = ceilingTileSize,
		.h = ceilingTileSize
	};
}

void GridCeiling::CollapseCeiling()
{
	ceilingLevel++;

	collider->SetNewBoxShape(
		Vector2F(ceilingTileSize * ceilingWidth, ceilingTileSize),
		Vector2F((ceilingTileSize * ceilingWidth) / 2, (-ceilingTileSize / 2) + (ceilingTileSize * ceilingLevel)),
		0.0f);
}

void GridCeiling::ResetCeiling()
{
	ceilingLevel = 0;

	collider->SetNewBoxShape(
		Vector2F(ceilingTileSize * ceilingWidth, ceilingTileSize),
		Vector2F((ceilingTileSize * ceilingWidth) / 2, (-ceilingTileSize / 2) + (ceilingTileSize * ceilingLevel)),
		0.0f);
}

int GridCeiling::GetCeilingLevel()
{
	return ceilingLevel;
}