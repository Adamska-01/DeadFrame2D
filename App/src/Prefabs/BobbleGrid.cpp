#include "Components/BobbleController.h"
#include "Components/Transform.h"
#include "Math/Vector2.h"
#include "Models/BobbleGridLevel.h"
#include "Prefabs/Bobble.h"
#include "Prefabs/BobbleGrid.h"
#include <Components/Collisions/Collider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <random>
#include <Tools/JsonSerializer.h>
#include <Tools/Helpers/EventHelpers.h>


BobbleGrid::BobbleGrid()
{
	bobbleSize = 1;
	transform = nullptr;

	positionToBobble.clear();
	bobbleToPosition.clear();

	if (!JsonSerializer::IsSerializable<BobbleGridLevel>())
	{
		throw std::runtime_error("BobbleGridLevel is not serializable.");
	}
}

void BobbleGrid::DestroyGridLevel()
{
	auto bobbles = OwningObject.lock()->GetChildren();

	for (auto& bobble : bobbles)
	{
		if (auto b = bobble.lock())
		{
			b->Destroy();
		}
	}

	bobbles.clear();
}

void BobbleGrid::OnGridBobbleCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.thisCollider == nullptr 
		|| collisionInfo.thisCollider->GetGameObject().lock() == nullptr
		|| collisionInfo.otherCollider == nullptr
		|| collisionInfo.otherCollider->GetGameObject().lock() == nullptr)
		return;

	auto thisBobble = collisionInfo.thisCollider->GetGameObject().lock();
	auto otherBobble = collisionInfo.otherCollider->GetGameObject().lock();
	auto thisBobbleController = thisBobble->GetComponent<BobbleController>();
	auto otherBobbleController = otherBobble->GetComponent<BobbleController>();

	if (thisBobbleController == nullptr 
		|| otherBobbleController == nullptr
		|| otherBobbleController->IsPartOfGrid())
		return;

	auto otherTransform = otherBobble->GetComponent<Transform>();
	auto thisTransform = thisBobble->GetComponent<Transform>();

	auto fromThisToOther = (otherTransform->GetWorldPosition() - thisTransform->GetWorldPosition()).Normalize();

	static const std::array<Vector2F, 6> HEX_DIRECTIONS = 
	{
		Vector2F(std::cos(MathConstants::PI / 3), -std::sin(MathConstants::PI / 3)),	// TOPRIGHT
		Vector2F(1, 0),																	// RIGHT
		Vector2F(std::cos(MathConstants::PI / 3), std::sin(MathConstants::PI / 3)),		// BOTTOMRIGHT
		Vector2F(-std::cos(MathConstants::PI / 3), std::sin(MathConstants::PI / 3)),	// BOTTOMLEFT
		Vector2F(-1, 0),																// LEFT
		Vector2F(-std::cos(MathConstants::PI / 3), -std::sin(MathConstants::PI / 3))	// TOPLEFT
	};

	auto bestDirectionIndex = 0;
	auto maxDot = -std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < HEX_DIRECTIONS.size(); ++i)
	{
		auto dot = Vector2F::Dot(fromThisToOther, HEX_DIRECTIONS[i]);
		
		if (dot > maxDot)
		{
			maxDot = dot;
			bestDirectionIndex = i;
		}
	}

	const auto& [row, col] = bobbleToPosition[thisBobble];

	auto newBobbleCord = GetNeighborCoord(row, col, (BobbleConnectionDirection)bestDirectionIndex);

	auto otherRigidBody = otherBobble->GetComponent<RigidBody2D>();

	otherRigidBody->SetVelocity(Vector2F::Zero);
	otherRigidBody->ChangeBodyType(BodyType2D::Static);

	auto placement = transform->GetWorldPosition();

	auto displacement = (newBobbleCord.value().first % 2) * bobbleSize / 2;
	auto x = placement.x + newBobbleCord.value().second * bobbleSize + displacement;
	auto y = placement.y + newBobbleCord.value().first * bobbleSize;

	otherTransform->SetWorldPosition(Vector2F(x + bobbleSize / 2.0f, y + bobbleSize / 2.0f));

	otherBobbleController->SetHanging(row == 0);
	otherBobbleController->SetPartOfGrid(true);
}

std::optional<std::pair<int, int>> BobbleGrid::GetNeighborCoord(int row, int col, BobbleConnectionDirection direction)
{
	auto isOddRow = (row % 2) != 0;

	switch (direction)
	{
	case BobbleConnectionDirection::TOP_RIGHT:
		return std::make_pair(row - 1, isOddRow ? col + 1 : col);
	case BobbleConnectionDirection::RIGHT:
		return std::make_pair(row, col + 1);
	case BobbleConnectionDirection::BOTTOM_RIGHT:
		return std::make_pair(row + 1, isOddRow ? col + 1 : col);
	case BobbleConnectionDirection::BOTTOM_LEFT:
		return std::make_pair(row + 1, isOddRow ? col : col - 1);
	case BobbleConnectionDirection::LEFT:
		return std::make_pair(row, col - 1);
	case BobbleConnectionDirection::TOP_LEFT:
		return std::make_pair(row - 1, isOddRow ? col : col - 1);
	default:
		return std::nullopt;
	}
}

void BobbleGrid::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
}

void BobbleGrid::Update(float deltaTime)
{
}

void BobbleGrid::Draw()
{
}

void BobbleGrid::SetNewGridLevel(std::string_view levelSource, int tileSize)
{
	DestroyGridLevel();

	auto levelData = JsonSerializer::DeserializeFromFile<BobbleGridLevel>(levelSource);

	bobbleSize = tileSize;

	// TODO: Set the color in the grid data source itself
	static std::random_device rd;  // Seed for random number engine
	static std::mt19937 gen(rd()); // Mersenne Twister PRNG
	static std::uniform_int_distribution<int> dist(0, static_cast<int>(BobbleColor::ALL_COLOURS) - 1);

	auto placement = transform->GetWorldPosition();

	for (auto i = 0; i < levelData.height; i++)
	{
		auto displacement = (i % 2) * tileSize / 2;
		auto start = i * levelData.width;
		auto end = start + (levelData.width - 1) - (i % 2);

		for (auto j = start; j <= end; j++)
		{
			if (levelData.grid[j] <= 0)
				continue;

			auto x = placement.x + (j - start) * tileSize + displacement;
			auto y = placement.y + i * tileSize;

			auto bobble = GameObject::Instantiate<Bobble>(Vector2F{ x + tileSize / 2.0f, y + tileSize / 2.0f }, static_cast<BobbleColor>(dist(gen)));
			auto coord = std::make_pair(i, j % levelData.width);
			
			positionToBobble[coord] = bobble;
			bobbleToPosition[bobble] = coord;

			OwningObject.lock()->AddChildGameObject(bobble);

			// Set Collision handlers 
			auto collider = bobble.lock()->GetComponent<Collider2D>();
			collider->RegisterCollisionEnterHandler(EventHelpers::BindFunction(this, &BobbleGrid::OnGridBobbleCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
		}
	}
	
	for (const auto& [position, weakBobble] : positionToBobble)
	{
		const auto& [row, col] = position;
		auto bobbleConnections = weakBobble.lock()->GetComponent<BobbleController>();

		assert(bobbleConnections != nullptr && "Bobble has no 'BobbleConnections' component!");

		bobbleConnections->SetHanging(row == 0);
		bobbleConnections->SetPartOfGrid(true);
		
		static const std::array<BobbleConnectionDirection, 6> BOBBLE_CONNECTION_DIRECTIONS
		{
			BobbleConnectionDirection::TOP_RIGHT,
			BobbleConnectionDirection::RIGHT,
			BobbleConnectionDirection::BOTTOM_RIGHT,
			BobbleConnectionDirection::BOTTOM_LEFT,
			BobbleConnectionDirection::LEFT,
			BobbleConnectionDirection::TOP_LEFT
		};

		for (auto dir : BOBBLE_CONNECTION_DIRECTIONS)
		{
			auto neighborCoordOpt = GetNeighborCoord(row, col, dir);
			
			if (!neighborCoordOpt.has_value())
				continue;
			
			auto it = positionToBobble.find(neighborCoordOpt.value());
			
			if (it == positionToBobble.end())
				continue;

			bobbleConnections->SetConnectionAt(dir, it->second);
		}
	}
}