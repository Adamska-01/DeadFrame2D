#pragma once
#include "Components/Collisions/BoxCollider2D.h"
#include "Factories/Abstractions/Debugging/IDebugColliderDrawer.h"


class DebugBoxColliderDrawer : public IDebugColliderDrawer<BoxCollider2D>
{
public:
	DebugBoxColliderDrawer(const BoxCollider2D* collider);


	virtual void DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled = false) override;

	virtual void DrawCollider(const SDL_Color& drawColor, bool filled = false) override;
};