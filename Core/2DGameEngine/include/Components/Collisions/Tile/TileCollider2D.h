#pragma once
#include "Components/Collisions/Collider2D.h"


template <typename TRenderer>
class TileCollider2D : public Collider2D
{
protected:
	TRenderer* tileMapRenderer;
	
	
	TileCollider2D(const PhysicsMaterial& physicsMaterial = PhysicsMaterial())
		: Collider2D(physicsMaterial), tileMapRenderer(nullptr)
	{
	}

	virtual ~TileCollider2D() = default;
};