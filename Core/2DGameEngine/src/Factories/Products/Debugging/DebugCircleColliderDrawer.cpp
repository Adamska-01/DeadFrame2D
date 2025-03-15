#include "Components/Collisions/CircleCollider2D.h"
#include "Factories/Products/Debugging/DebugCircleColliderDrawer.h"
#include "SubSystems/TextureManager.h"


DebugCircleColliderDrawer::DebugCircleColliderDrawer(const CircleCollider2D* collider)
{
	this->collider = collider;
}

void DebugCircleColliderDrawer::DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled)
{
	TextureManager::DrawCircle(collider->GetCircle(), SDL_Color(r, g, b, a), filled);
}

void DebugCircleColliderDrawer::DrawCollider(const SDL_Color& drawColor, bool filled)
{
	TextureManager::DrawCircle(collider->GetCircle(), drawColor, filled);
}