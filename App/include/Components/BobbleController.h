#pragma once
#include "Components/GameComponent.h"
#include "Constants/BobbleConstants.h"
#include "Generic/Bobble/BobbleConnectionDirection.h"
#include "Generic/Bobble/BobbleColor.h"
#include <array>
#include <memory>


class GameObject;
class SpriteAnimator;


class BobbleController : public GameComponent
{
private:
	bool partOfGrid;

	BobbleColor bobbleColor;

	std::array<std::weak_ptr<GameObject>, BobbleConstants::MAX_BOBBLE_NEIGHBOURS> connectionList;

	SpriteAnimator* spriteAnimator;


public:
	BobbleController(BobbleColor bobbleColor);

	virtual ~BobbleController() override = default;
	

	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	BobbleColor GetBobbleColor() const;

	bool IsPartOfGrid() const;

	std::weak_ptr<GameObject> GetConnectionAt(BobbleConnectionDirection connectionDirection) const;

	void SetColor(BobbleColor newColor);

	void SetPartOfGrid(bool partOfGrid);

	void SetConnectionAt(BobbleConnectionDirection connectionDirection, std::weak_ptr<GameObject> connection);
};