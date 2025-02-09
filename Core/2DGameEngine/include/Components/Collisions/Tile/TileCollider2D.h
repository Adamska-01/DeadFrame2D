#pragma once
#include "Components/Collisions/Collider2D.h"


template <typename TRenderer>
class TileCollider2D : public Collider2D
{
protected:
	TRenderer* tileMapRenderer;
	
	
	TileCollider2D() = default;


public:
	virtual ~TileCollider2D() = default;


	virtual void Init() = 0;

	virtual void Update(float dt) = 0;

	virtual void Draw() = 0;

	virtual void Clean() = 0;

	
	virtual bool Accept(class ColliderVisitor& visitor, Collider2D& other) = 0;
};