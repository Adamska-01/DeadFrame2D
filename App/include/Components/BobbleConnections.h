#pragma once
#include "Components/GameComponent.h"
#include "Constants/BobbleConstants.h"
#include "Generic/Bobble/BobbleConnectionDirection.h"
#include <array>
#include <memory>


class GameObject;


class BobbleConnections : public GameComponent
{
private:
	std::array<std::weak_ptr<GameObject>, BobbleConstants::MAX_BOBBLE_NEIGHBOURS> connectionList;


public:
	BobbleConnections();

	virtual ~BobbleConnections() override = default;
	

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection);
};