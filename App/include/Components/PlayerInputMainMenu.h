#pragma once
#include <Components/GameComponent.h>


class Transform;
class MenuManager;


class PlayerInputMainMenu : public GameComponent
{
private:
	Transform* transform;

	MenuManager* menuManager;


public:
	PlayerInputMainMenu();

	virtual ~PlayerInputMainMenu() override = default;


	virtual void Init() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};