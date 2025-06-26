#pragma once
#include "Generic//Bobble/BobbleConnectionDirection.h"
#include "Generic/Bobble/BobbleColor.h"
#include "Math/Vector2.h"
#include "Models/BobbleGridLevel.h"
#include <Components/GameComponent.h>
#include <Data/Collision/CollisionInfo.h>
#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <Tools/Hashing/WeakPtrEqual.h>
#include <Tools/Hashing/WeakPtrHash.h>
#include <unordered_map>
#include <unordered_set>


class GameObject;
class Transform;
class TiledMapCompatibleCollider2D;
class Cannon;
class GridCeiling;
class GameManager;


class BobbleGrid : public GameComponent
{
private:
	int bobbleSize;

	int shotCount;

	int maxHeight;

	Transform* transform;

	GameManager* gameManager;

	Cannon* cannon;

	GridCeiling* gridCeiling;

	BobbleGridLevel currentLevelSpec;

	std::map<Vector2I, std::weak_ptr<GameObject>> positionToBobble;

	std::unordered_map<std::weak_ptr<GameObject>, Vector2I, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbleToPosition;


	void DestroyGridLevel();

	void RemoveAndDestroyBobbles(std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbles, bool canPop = true);

	void PopulateBobbleConnections();

	void OnGridBobbleCollisionEnterHandler(const CollisionInfo& collisionInfo);

	std::optional<Vector2I> GetNeighborCoord(Vector2I coord, BobbleConnectionDirection direction) const;

	void PlaceBobbleAdjacentTo(const std::shared_ptr<GameObject>& referenceBobble, const std::shared_ptr<GameObject>& newBobble);

	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> FindConnectedSameColorBobbles(const std::shared_ptr<GameObject>& startBobble) const;

	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> FindDisconnectedBobbles() const;


public:
	BobbleGrid();

	virtual ~BobbleGrid() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void OnCeilingCollisionEnterHandler(const CollisionInfo& collisionInfo);


	void SetNewGridLevel(std::string_view levelSource, int tileSize);

	std::set<BobbleColor> GetAvailableColors();

	BobbleGridLevel GetCurrentLevel();
};