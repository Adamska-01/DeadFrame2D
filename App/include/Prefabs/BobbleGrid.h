#pragma once
#include <Components/GameComponent.h>
#include <map>
#include <Tools/Hashing/WeakPtrEqual.h>
#include <Tools/Hashing/WeakPtrHash.h>
#include <unordered_map>
#include <utility>


class GameObject;
class Transform;


class BobbleGrid : public GameComponent
{
private:
	Transform* transform;

	std::map<std::pair<int, int>, std::weak_ptr<GameObject>> positionToBobble;

	std::unordered_map<std::weak_ptr<GameObject>, std::pair<int, int>, WeakPtrHash<GameObject>, WeakPtrEqual<GameObject>> bobbleToPosition;


	void DestroyGridLevel();


public:
	BobbleGrid();

	virtual ~BobbleGrid() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetNewGridLevel(std::string_view levelSource, int tileSize);
};