#pragma once
#include <Components/GameComponent.h>
#include <memory>


class Transform;
class Bobble;


class Cannon : public GameComponent
{
private:
	Transform* transform;

	std::weak_ptr<Bobble> loadedBobble;


public:
	Cannon();

	virtual ~Cannon() override;


	virtual void Init() override;

	virtual void Update(float deltaTime) override;

	virtual void Draw() override;


	void Fire();
};