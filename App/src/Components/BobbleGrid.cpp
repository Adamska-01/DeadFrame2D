#include "Components/BobbleController.h"
#include "Components/BobbleGrid.h"
#include "Components/Cannon.h"
#include "Components/GameFlow/GameManager.h"
#include "Components/GridCeiling.h"
#include "Components/Transform.h"
#include "Constants/AssetPaths.h"
#include "Constants/BobbleConstants.h"
#include "Events/LoadNewBobbleEvent.h"
#include "Prefabs/AudioClipObject.h"
#include "Prefabs/Bobble.h"
#include <Components/Collisions/Collider2D.h>
#include <Components/Physics/RigidBody2D.h>
#include <Coroutines/CoroutineScheduler.h>
#include <GameObject.h>
#include <queue>
#include <Tools/Helpers/EventHelpers.h>
#include <Tools/Helpers/Guards.h>
#include <Tools/JsonSerializer.h>
#include <unordered_set>


BobbleGrid::BobbleGrid()
	: bobbleSize(1), 
	shotCount(0),
	maxHeight(0),
	transform(nullptr),
	gameManager(nullptr), 
	cannon(nullptr),
	gridCeiling(nullptr)
{
	positionToBobble.clear();
	bobbleToPosition.clear();

	if (!JsonSerializer::IsSerializable<BobbleGridLevel>())
	{
		throw std::runtime_error("BobbleGridLevel is not serializable.");
	}
}

void BobbleGrid::DestroyGridLevel()
{
	for (auto& bobble : positionToBobble)
	{
		if (auto b = bobble.second.lock())
		{
			b->Destroy();
		}
	}

	positionToBobble.clear();
	bobbleToPosition.clear();
}

void BobbleGrid::RemoveAndDestroyBobbles(std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbles, bool canPop)
{
	auto scoreMuliplier = canPop ? BobbleConstants::SCORE_MULTIPLIER_POP : BobbleConstants::SCORE_MULTIPLIER_DROP;
	auto score = bobbles.size() * scoreMuliplier;
	
	gameManager->AddScore(score);

	if (score > 0 && canPop)
	{
		auto soundSourceObj = GameObject::Instantiate<AudioClipObject>(
			AssetPaths::Audio::BOBBLE_POP,
			Vector2F::Zero,
			0.4f,
			true);

		CoroutineScheduler::StartCoroutine(soundSourceObj.lock()->Destroy(1.0f));
	}

	for (const auto& bobble : bobbles)
	{
		auto bobblePtr = bobble.lock();

		if (bobblePtr == nullptr)
			continue;

		auto it = bobbleToPosition.find(bobble);

		if (it == bobbleToPosition.end())
			continue;

		positionToBobble.erase(it->second);
		bobbleToPosition.erase(it->first);

		if (canPop)
		{
			bobblePtr->GetComponent<BobbleController>()->PopBobble();
		}
		else
		{
			bobblePtr->GetComponent<BobbleController>()->DropBobble();
		}
	}
}

void BobbleGrid::PopulateBobbleConnections()
{
	for (const auto& [position, weakBobble] : positionToBobble)
	{
		auto bobbleConnections = weakBobble.lock()->GetComponent<BobbleController>();

		Tools::Helpers::GuardAgainstNull(bobbleConnections, "Bobble has no 'BobbleConnections' component!");

		bobbleConnections->SetPartOfGrid(true);

		for (auto dir : BobbleConstants::BOBBLE_CONNECTION_DIRECTIONS)
		{
			auto neighborCoordOpt = GetNeighborCoord(position, dir);

			if (!neighborCoordOpt.has_value())
				continue;

			auto it = positionToBobble.find(neighborCoordOpt.value());

			if (it == positionToBobble.end())
				continue;

			bobbleConnections->SetConnectionAt(dir, it->second);
		}
	}
}

void BobbleGrid::OnGridBobbleCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.thisGameObject.expired() || collisionInfo.otherGameObject.expired())
		return;

	auto thisBobble = collisionInfo.thisGameObject.lock();
	auto otherBobble = collisionInfo.otherGameObject.lock();
	auto thisBobbleController = thisBobble->GetComponent<BobbleController>();
	auto otherBobbleController = otherBobble->GetComponent<BobbleController>();

	if (thisBobbleController == nullptr 
		|| otherBobbleController == nullptr
		|| otherBobbleController->IsPartOfGrid()
		|| otherBobbleController->IsDestructionPending()
		|| thisBobbleController->IsDestructionPending())
		return;

	auto rigidBody = otherBobble->GetComponent<RigidBody2D>();

	if (rigidBody != nullptr)
	{
		rigidBody->ChangeBodyType(BodyType2D::Static);
	}

	auto collider = otherBobble->GetComponent<Collider2D>();
	
	if (collider != nullptr)
	{
		collider->RegisterContactEnterHandler(EventHelpers::BindFunction(this, &BobbleGrid::OnGridBobbleCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
	}

	PlaceBobbleAdjacentTo(thisBobble, otherBobble);

	RemoveAndDestroyBobbles(FindConnectedSameColorBobbles(otherBobble));

	RemoveAndDestroyBobbles(FindDisconnectedBobbles(), false);

	PopulateBobbleConnections();

	MarkDirty();
}

std::optional<Vector2I> BobbleGrid::GetNeighborCoord(Vector2I coord, BobbleConnectionDirection direction) const
{
	auto isOddRow = (coord.x % 2) != 0;

	switch (direction)
	{
	case BobbleConnectionDirection::TOP_RIGHT:
		return Vector2I(coord.x - 1, isOddRow ? coord.y + 1 : coord.y);
	case BobbleConnectionDirection::RIGHT:
		return Vector2I(coord.x, coord.y + 1);
	case BobbleConnectionDirection::BOTTOM_RIGHT:
		return Vector2I(coord.x + 1, isOddRow ? coord.y + 1 : coord.y);
	case BobbleConnectionDirection::BOTTOM_LEFT:
		return Vector2I(coord.x + 1, isOddRow ? coord.y : coord.y - 1);
	case BobbleConnectionDirection::LEFT:
		return Vector2I(coord.x, coord.y - 1);
	case BobbleConnectionDirection::TOP_LEFT:
		return Vector2I(coord.x - 1, isOddRow ? coord.y : coord.y - 1);
	default:
		return std::nullopt;
	}
}

void BobbleGrid::PlaceBobbleAdjacentTo(const std::shared_ptr<GameObject>& referenceBobble, const std::shared_ptr<GameObject>& newBobble)
{
	auto referenceTransform = referenceBobble->GetComponent<Transform>();
	auto newTransform = newBobble->GetComponent<Transform>();
	auto newRigidBody = newBobble->GetComponent<RigidBody2D>();
	auto newBobbleController = newBobble->GetComponent<BobbleController>();

	auto fromRefToNew = (newTransform->GetWorldPosition() - referenceTransform->GetWorldPosition()).Normalize();

	int bestDirectionIndex = 0;
	float maxDot = -std::numeric_limits<float>::infinity();

	for (size_t i = 0; i < BobbleConstants::BOBBLE_HEX_DIRECTIONS.size(); ++i)
	{
		auto dot = Vector2F::Dot(fromRefToNew, BobbleConstants::BOBBLE_HEX_DIRECTIONS[i]);
		if (dot > maxDot)
		{
			maxDot = dot;
			bestDirectionIndex = static_cast<int>(i);
		}
	}

	auto newCoord = GetNeighborCoord(bobbleToPosition[referenceBobble], static_cast<BobbleConnectionDirection>(bestDirectionIndex)).value();

	newRigidBody->SetVelocity(Vector2F::Zero);
	newRigidBody->ChangeBodyType(BodyType2D::Static);

	auto placementOrigin = transform->GetWorldPosition() + Vector2F::Down * bobbleSize * gridCeiling->GetCeilingLevel();
	auto displacement = (newCoord.x % 2) * bobbleSize / 2;
	auto x = placementOrigin.x + newCoord.y * bobbleSize + displacement;
	auto y = placementOrigin.y + newCoord.x * bobbleSize;

	newTransform->SetWorldPosition(Vector2F(x + bobbleSize / 2.0f, y + bobbleSize / 2.0f));

	auto a = newTransform->GetWorldPosition();

	newBobbleController->SetPartOfGrid(true);

	positionToBobble[newCoord] = newBobble;
	bobbleToPosition[newBobble] = newCoord;
}

std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> BobbleGrid::FindConnectedSameColorBobbles(const std::shared_ptr<GameObject>& startBobble) const
{
	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> result = {};

	auto it = bobbleToPosition.find(startBobble);

	if (it == bobbleToPosition.end())
		return result;

	auto[startRow, startCol] = it->second;

	auto startController = startBobble->GetComponent<BobbleController>();

	if (startController == nullptr)
		return result;

	const auto targetColor = startController->GetBobbleColor();

	std::queue<Vector2I> toVisit;
	std::unordered_set<Vector2I> visited;

	toVisit.emplace(Vector2I(startRow, startCol));
	visited.emplace(Vector2I(startRow, startCol));

	while (!toVisit.empty())
	{
		auto[row, col] = toVisit.front();
		
		toVisit.pop();

		auto coord = Vector2I(row, col);
		
		auto itBobble = positionToBobble.find(coord);
		
		if (itBobble == positionToBobble.end() || itBobble->second.lock() == nullptr)
			continue;

		const auto& bobble = itBobble->second;

		auto controller = bobble.lock()->GetComponent<BobbleController>();
		
		if (controller == nullptr || controller->GetBobbleColor() != targetColor)
			continue;

		result.insert(bobble);

		for (auto dir : BobbleConstants::BOBBLE_CONNECTION_DIRECTIONS)
		{
			auto neighborOpt = GetNeighborCoord(coord, dir);
		
			if (!neighborOpt.has_value())
				continue;

			auto neighborCoord = neighborOpt.value();

			if (visited.insert(neighborCoord).second)
			{
				toVisit.push(neighborCoord);
			}
		}
	}

	if (result.size() < BobbleConstants::MIN_BOBBLE_TO_SCORE)
	{
		result.clear();
	}

	return result;
}

std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> BobbleGrid::FindDisconnectedBobbles() const
{
	std::queue<Vector2I> toVisit;
	std::unordered_set<Vector2I> visited;

	// Start from top row (Only add the top ones)
	for (const auto& [position, bobble] : positionToBobble)
	{
		if (position.x != 0 || bobble.lock() == nullptr)
			continue;

		toVisit.push(position);
		visited.insert(position);
	}

	// Find all connected bobbles
	while (!toVisit.empty())
	{
		auto coord = toVisit.front();
		toVisit.pop();

		for (auto dir : BobbleConstants::BOBBLE_CONNECTION_DIRECTIONS)
		{
			auto neighborOpt = GetNeighborCoord(coord, dir);
			
			if (!neighborOpt.has_value())
				continue;

			const auto& neighborCoord = neighborOpt.value();

			auto it = positionToBobble.find(neighborCoord);
			
			if (it != positionToBobble.end() 
				&& it->second.lock() != nullptr
				&& visited.insert(neighborCoord).second) // if the element was not already in the set
			{
				toVisit.push(neighborCoord);
			}
		}
	}

	// Get everything that has not been visited (disconnected)
	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> disconnected;

	for (const auto& [coord, bobble] : positionToBobble)
	{
		if (visited.find(coord) != visited.end() || bobble.lock() == nullptr)
			continue;
		
		disconnected.insert(bobble);
	}

	return disconnected;
}

void BobbleGrid::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
	cannon = OwningObject.lock()->GetParent().lock()->GetComponentInChildren<Cannon>();
	gameManager = OwningObject.lock()->GetComponentInParent<GameManager>();
	gridCeiling = OwningObject.lock()->GetComponentInChildren<GridCeiling>();

	Tools::Helpers::GuardAgainstNull(transform, "BobbleGrid::Init: Transform not found from BobbleGrid.");
	Tools::Helpers::GuardAgainstNull(cannon, "BobbleGrid::Init: Cannon not found from BobbleGrid.");
	Tools::Helpers::GuardAgainstNull(gameManager, "BobbleGrid::Init: GameManager not found from BobbleGrid.");
	Tools::Helpers::GuardAgainstNull(gridCeiling, "BobbleGrid::Init: GridCeiling not found from BobbleGrid.");
}

void BobbleGrid::Start()
{
	EventDispatcher::SendEvent(std::make_shared<LoadNewBobbleEvent>(cannon));
}

void BobbleGrid::Update(float deltaTime)
{
	if (!isDirty)
		return;

	shotCount++;

	if (positionToBobble.size() <= 0)
	{
		CoroutineScheduler::StartCoroutine(gameManager->StartNextLevel(true));

		isDirty = false;
		
		return;
	}

	if (shotCount > BobbleConstants::SHOTS_REQUIRED_FOR_CEALING_FALL)
	{
		gridCeiling->CollapseCeiling();

		for (auto& [pos, bobbleWeak] : positionToBobble)
		{
			auto bobble = bobbleWeak.lock();
			
			if (bobble == nullptr)
				continue;

			bobble->GetTransform()->TranslateY(bobbleSize);
		}

		shotCount = 0;
		maxHeight--;
	}

	for (const auto& [pos, bobble] : positionToBobble)
	{
		if (pos.x < maxHeight)
			continue;

		gameManager->ShowGameOverScreen();

		isDirty = false;

		return;
	}

	EventDispatcher::SendEvent(std::make_shared<LoadNewBobbleEvent>(cannon));
		
	isDirty = false;
}

void BobbleGrid::Draw()
{
}

void BobbleGrid::OnCeilingCollisionEnterHandler(const CollisionInfo& collisionInfo)
{
	if (collisionInfo.otherGameObject.expired())
		return;

	auto otherBobble = collisionInfo.otherGameObject.lock();
	auto otherBobbleController = otherBobble->GetComponent<BobbleController>();

	if (otherBobbleController == nullptr
		|| otherBobbleController->IsPartOfGrid()
		|| otherBobbleController->IsDestructionPending())
		return;

	auto newRigidBody = otherBobble->GetComponent<RigidBody2D>();

	newRigidBody->SetVelocity(Vector2F::Zero);
	newRigidBody->ChangeBodyType(BodyType2D::Static);

	auto bobblePos = otherBobble->GetComponent<Transform>()->GetWorldPosition();

	auto coordX = static_cast<int>((bobblePos.x - transform->GetWorldPosition().x) / bobbleSize);
	auto coordY = 0;

	auto placementOrigin = transform->GetWorldPosition() + Vector2F::Down * bobbleSize * gridCeiling->GetCeilingLevel();
	auto displacement = (coordY % 2) * bobbleSize / 2;
	auto x = placementOrigin.x + coordX * bobbleSize + displacement;
	auto y = placementOrigin.y + coordY * bobbleSize;

	otherBobble->GetComponent<Transform>()->SetWorldPosition(Vector2F(x + bobbleSize / 2.0f, y + bobbleSize / 2.0f));

	auto coord = Vector2I(coordY, coordX);

	positionToBobble[coord] = otherBobble;
	bobbleToPosition[otherBobble] = coord;

	otherBobbleController->SetPartOfGrid(true);

	auto rigidBody = otherBobble->GetComponent<RigidBody2D>();

	if (rigidBody != nullptr)
	{
		rigidBody->ChangeBodyType(BodyType2D::Static);
	}

	auto collider = otherBobble->GetComponent<Collider2D>();

	if (collider != nullptr)
	{
		collider->RegisterContactEnterHandler(EventHelpers::BindFunction(this, &BobbleGrid::OnGridBobbleCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
	}

	PopulateBobbleConnections();

	RemoveAndDestroyBobbles(FindConnectedSameColorBobbles(otherBobble));

	RemoveAndDestroyBobbles(FindDisconnectedBobbles(), false);

	PopulateBobbleConnections();

	MarkDirty();
}

void BobbleGrid::SetNewGridLevel(std::string_view levelSource, int tileSize)
{
	DestroyGridLevel();

	currentLevelSpec = JsonSerializer::DeserializeFromFile<BobbleGridLevel>(levelSource);

	bobbleSize = tileSize;
	maxHeight = currentLevelSpec.height;

	gridCeiling->SetCeilingTileSize(tileSize);

	auto placement = transform->GetWorldPosition();

	for (auto i = 0; i < currentLevelSpec.height; i++)
	{
		auto displacement = (i % 2) * tileSize / 2;
		auto start = i * currentLevelSpec.width;
		auto end = start + (currentLevelSpec.width - 1) - (i % 2);

		for (auto j = start; j <= end; j++)
		{
			auto color = currentLevelSpec.grid[j];

			if (currentLevelSpec.grid[j] >= (int)BobbleColor::MAX_COLORS)
				continue;

			auto x = placement.x + (j - start) * tileSize + displacement;
			auto y = placement.y + i * tileSize;

			auto bobble = GameObject::Instantiate<Bobble>(Vector2F{ x + tileSize / 2.0f, y + tileSize / 2.0f }, static_cast<BobbleColor>(color));
			auto coord = Vector2I(i, j % currentLevelSpec.width);
			
			positionToBobble[coord] = bobble;
			bobbleToPosition[bobble] = coord;

			// Set Collision handlers 
			auto collider = bobble.lock()->GetComponent<Collider2D>();
			collider->RegisterContactEnterHandler(EventHelpers::BindFunction(this, &BobbleGrid::OnGridBobbleCollisionEnterHandler), reinterpret_cast<uintptr_t>(this));
		}
	}
	
	PopulateBobbleConnections();

	shotCount = 0;
}

std::set<BobbleColor> BobbleGrid::GetAvailableColors()
{
	std::set<BobbleColor> result;

	for (const auto& [coord, bobble] : positionToBobble)
	{
		auto bobblePtr = bobble.lock();

		if (bobblePtr == nullptr)
			continue;

		auto bobbleController = bobble.lock()->GetComponent<BobbleController>();

		if (bobbleController == nullptr)
			continue;

		result.insert(bobbleController->GetBobbleColor());
	}

	return result;
}

BobbleGridLevel BobbleGrid::GetCurrentLevel()
{
	return currentLevelSpec;
}