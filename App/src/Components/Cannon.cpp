#include "Components/Cannon.h"
#include "Prefabs/Bobble.h"
#include <Components/Transform.h>


Cannon::Cannon()
{
	transform = nullptr;
}

Cannon::~Cannon()
{
	auto bobblePtr = loadedBobble.lock();

	if (bobblePtr == nullptr)
		return;

	bobblePtr->Destroy();
}

void Cannon::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
}

void Cannon::Update(float deltaTime)
{
	auto bobblePtr = loadedBobble.lock();

	if (bobblePtr == nullptr)
	{
		// TODO: The color needs to be set based on the available boobles in the grid
		loadedBobble = GameObject::Instantiate<Bobble>(transform->GetWorldPosition(), BobbleColor::Blue);
	}
}

void Cannon::Draw()
{
}

void Cannon::Fire()
{
	// TODO: Fire Bobble
}