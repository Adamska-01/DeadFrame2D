#pragma once
#include "Components/Collisions/Collider2D.h"


template <typename TRenderer>
class TileCollider2D : public Collider2D
{
protected:
	TRenderer* tileMapRenderer;
	
	
	TileCollider2D()
		: tileMapRenderer(nullptr)
	{
	}


public:
	virtual ~TileCollider2D() = default;


	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) = 0;
};