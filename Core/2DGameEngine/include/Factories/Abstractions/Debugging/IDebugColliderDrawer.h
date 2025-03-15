#pragma once
#include <cstdint>


struct SDL_Color;


template <typename T>
class IDebugColliderDrawer
{
protected:
	const T* collider;


public:
	virtual void DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled = false) = 0;
	
	virtual void DrawCollider(const SDL_Color& drawColor, bool filled = false) = 0;
};