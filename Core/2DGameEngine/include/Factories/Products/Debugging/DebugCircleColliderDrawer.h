#pragma once
#include "Factories/Abstractions/Debugging/IDebugColliderDrawer.h"


class CircleCollider2D;


class DebugCircleColliderDrawer : public IDebugColliderDrawer<CircleCollider2D>
{
public:
	DebugCircleColliderDrawer(const CircleCollider2D* collider);


	virtual void DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled = false) override;

	virtual void DrawCollider(const SDL_Color& drawColor, bool filled = false) override;
};