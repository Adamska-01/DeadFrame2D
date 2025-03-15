#pragma once
#include "Factories/Abstractions/Debugging/IDebugColliderDrawer.h"


template <typename T>
class EmptyColliderDrawer : public IDebugColliderDrawer<T>
{
	virtual void DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled = false) override;

	virtual void DrawCollider(const SDL_Color& drawColor, bool filled = false) override;
};


template<typename T>
inline void EmptyColliderDrawer<T>::DrawCollider(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool filled)
{
}

template<typename T>
inline void EmptyColliderDrawer<T>::DrawCollider(const SDL_Color& drawColor, bool filled)
{
}
