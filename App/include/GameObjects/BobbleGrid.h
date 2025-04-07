#pragma once
#include <GameObject.h>


class Bobble;


class BobbleGrid : public GameObject
{
private:
	std::vector<std::weak_ptr<Bobble>> bobbles;


	void CreateBobbleRow(int rowNumber);


public:
	BobbleGrid();

	virtual ~BobbleGrid() = default;


	virtual void Init() override;
};