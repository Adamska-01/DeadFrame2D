#pragma once
#include "Generic//Bobble/BobbleConnectionDirection.h"
#include <Components/GameComponent.h>
#include <Data/Collision/CollisionInfo.h>
#include <map>
#include <optional>
#include <string_view>
#include <Tools/Hashing/WeakPtrEqual.h>
#include <Tools/Hashing/WeakPtrHash.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>


class GameObject;
class Transform;


class BobbleGrid : public GameComponent
{
private:
	int bobbleSize;

	Transform* transform;
	//TODO: Convert all the pairs into Vector2I
	std::map<std::pair<int, int>, std::weak_ptr<GameObject>> positionToBobble;

	std::unordered_map<std::weak_ptr<GameObject>, std::pair<int, int>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbleToPosition;


	void DestroyGridLevel();
	
	void RemoveAndDestroyBobbles(std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbles, bool canPop = true);

	void OnGridBobbleCollisionEnterHandler(const CollisionInfo& collisionInfo);

	std::optional<std::pair<int, int>> GetNeighborCoord(int row, int col, BobbleConnectionDirection direction) const;

	void PlaceBobbleAdjacentTo(const std::shared_ptr<GameObject>& referenceBobble, const std::shared_ptr<GameObject>& newBobble);

	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> FindConnectedSameColorBobbles(const std::shared_ptr<GameObject>& startBobble) const;

	std::unordered_set<std::weak_ptr<GameObject>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> FindDisconnectedBobbles() const;


public:
	BobbleGrid();

	virtual ~BobbleGrid() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetNewGridLevel(std::string_view levelSource, int tileSize);
};