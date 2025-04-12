#include "Components/Cannon.h"
#include "Constants/TiledObjectGroupNames.h"
#include "Prefabs/Bobble.h"
#include <Components/GameMapParser.h>
#include <Components/Sprite.h>
#include <Components/Transform.h>
#include <Management/GameObjectRegistry.h>
#include <TileEditors/Tiled/Models/TiledObjectGroup.h>


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
	transform = OwningObject->GetComponent<Transform>();
}

void Cannon::Update(float deltaTime)
{
	auto bobblePtr = loadedBobble.lock();

	if (bobblePtr == nullptr)
	{
		// TODO: The color needs to be set based on the available boobles in the grid
		loadedBobble = GameObject::Instantiate<Bobble>(transform->position, BobbleColor::Blue);
	}
}

void Cannon::Draw()
{
}

void Cannon::Fire()
{
	// TODO: Fire Bobble
}
