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
	LoadBobble();
}

void Cannon::Draw()
{
}

void Cannon::Fire()
{
	// TODO: Fire Bobble
}

void Cannon::LoadBobble()
{
	if (loadedBobble.lock() != nullptr)
		return;

	// TODO: For now, randomize color. For later, get the color pick from the currently available colors in the grid.
	loadedBobble = GameObject::Instantiate<Bobble>(transform->GetWorldPosition(), BobbleColor::Blue);
}