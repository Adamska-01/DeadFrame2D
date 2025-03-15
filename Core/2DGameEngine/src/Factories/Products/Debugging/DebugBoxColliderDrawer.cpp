#include "Factories/Products/Debugging/DebugBoxColliderDrawer.h"
#include "SubSystems/TextureManager.h"


DebugBoxColliderDrawer::DebugBoxColliderDrawer(const BoxCollider2D* collider)
{
	this->collider = collider;
}

void DebugBoxColliderDrawer::DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled)
{
	TextureManager::DrawRect(collider->GetCollisionBox(), SDL_Color(r, g, b, a), filled);
}

void DebugBoxColliderDrawer::DrawCollider(const SDL_Color& drawColor, bool filled)
{
	TextureManager::DrawRect(collider->GetCollisionBox(), drawColor, filled);
}