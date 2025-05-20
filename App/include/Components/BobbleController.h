#pragma once
#include "Components/GameComponent.h"
#include "Constants/BobbleConstants.h"
#include "Generic/Bobble/BobbleConnectionDirection.h"
#include <array>
#include <memory>


class GameObject;


class BobbleController : public GameComponent
{
private:
	bool itsHanging;

	bool partOfGrid;

	std::array<std::weak_ptr<GameObject>, BobbleConstants::MAX_BOBBLE_NEIGHBOURS> connectionList;


public:
	BobbleController();

	virtual ~BobbleController() override = default;
	

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	bool IsPartOfGrid() const;

	bool IsHanging() const;

	void SetHanging(bool hanging);

	void SetPartOfGrid(bool partOfGrid);

	void SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection);
};