#include "Components/PlayerInput.h"
#include "Constants/PlayerInputConstants.h"
#include <algorithm>
#include <Components/Transform.h>
#include <GameObject.h>
#include <SubSystems/Input/Generic/PlayerInputSlot.h>
#include <SubSystems/Input/Input.h>


PlayerInput::PlayerInput()
{
	transform = nullptr;
}

void PlayerInput::ProcessRotationInput(float deltaTime)
{
	auto rotationDirection = 0.0f;

	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Left"))
	{
		rotationDirection = -1.0f;
	}
	if (Input::IsButtonHeld(PlayerInputSlot::PLAYER_1, "Right"))
	{
		rotationDirection = 1.0f;
	}

	transform->RotateByDegrees(rotationDirection * PlayerInputConstants::CANNON_ROTATION_SPEED * deltaTime);

	transform->SetLocalRotation(std::clamp(
		transform->GetLocalRotation(),
		PlayerInputConstants::CANNON_MIN_ROTATION, 
		PlayerInputConstants::CANNON_MAX_ROTATION));
}

void PlayerInput::ProcessFireInput()
{
}

void PlayerInput::Init()
{
	transform = OwningObject.lock()->GetComponent<Transform>();
}

void PlayerInput::Update(float deltaTime)
{
	ProcessRotationInput(deltaTime);
	ProcessFireInput();
}

void PlayerInput::Draw()
{
}