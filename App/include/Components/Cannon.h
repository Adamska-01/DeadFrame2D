#pragma once
#include <Components/GameComponent.h>
#include <memory>


class GameObject;
class Transform;


class Cannon : public GameComponent
{
private:
	Transform* transform;

	std::weak_ptr<GameObject> loadedBobble;


public:
	Cannon();

	virtual ~Cannon() override;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void Fire();

	void LoadBobble();
};