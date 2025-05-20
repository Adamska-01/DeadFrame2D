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
#include <utility>


class GameObject;
class Transform;


class BobbleGrid : public GameComponent
{
private:
	int bobbleSize;

	Transform* transform;

	std::map<std::pair<int, int>, std::weak_ptr<GameObject>> positionToBobble;

	std::unordered_map<std::weak_ptr<GameObject>, std::pair<int, int>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbleToPosition;


	void DestroyGridLevel();

	void OnGridBobbleCollisionEnterHandler(const CollisionInfo& collisionInfo);

	std::optional<std::pair<int, int>> GetNeighborCoord(int row, int col, BobbleConnectionDirection direction);


public:
	BobbleGrid();

	virtual ~BobbleGrid() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetNewGridLevel(std::string_view levelSource, int tileSize);
};