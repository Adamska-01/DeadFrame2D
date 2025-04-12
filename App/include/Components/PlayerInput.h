#pragma once
#include <Components/GameComponent.h>


class Transform;


class PlayerInput : public GameComponent
{
private:
	Transform* transform;


	void ProcessRotationInput(float deltaTime);

	void ProcessFireInput();


public:
	PlayerInput();

	virtual ~PlayerInput() override = default;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;
};