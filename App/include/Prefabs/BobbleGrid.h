#pragma once
#include "Math/Vector2.h"
#include "Models/BobbleGridLevel.h"
#include <GameObject.h>


class Bobble;


class BobbleGrid : public GameObject
{
private:
	int tileSize;

	Vector2F placement;

	BobbleGridLevel levelData;

	std::vector<std::weak_ptr<Bobble>> bobbles;


	void CreateBobbleRow(int row, int columnCount);


public:
	BobbleGrid(std::string_view levelSource, Vector2F placement, int tileSize);

	virtual ~BobbleGrid() = default;


	virtual void Init() override;
};