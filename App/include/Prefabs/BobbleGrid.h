#pragma once
#include "Math/Vector2.h"
#include <Components/GameComponent.h>


class Bobble;
class Transform;


class BobbleGrid : public GameComponent
{
private:
	Transform* transform;


	void DestroyGridLevel();


public:
	BobbleGrid();

	virtual ~BobbleGrid() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetNewGridLevel(std::string_view levelSource, int tileSize);
};