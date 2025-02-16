#pragma once
#include "Math/Vector2.h"
#include "Tools/Collisions/ICollisionVisitor.h"


class CollisionHandler : public ICollisionVisitor
{
public:
	CollisionHandler() = default;

	~CollisionHandler() = default;


	static bool PointInCircle(const Vector2F& point, const CircleCollider2D& circle);
	
	
	virtual bool Visit(const BoxCollider2D* box, const BoxCollider2D* other) override;

	virtual bool Visit(const BoxCollider2D* box, const CircleCollider2D* other) override;

	virtual bool Visit(const BoxCollider2D* box, const TiledMapCompatibleCollider2D* other) override;


	virtual bool Visit(const CircleCollider2D* circle, const BoxCollider2D* other) override;

	virtual bool Visit(const CircleCollider2D* circle, const CircleCollider2D* other) override;

	virtual bool Visit(const CircleCollider2D* circle, const TiledMapCompatibleCollider2D* other) override;


	virtual bool Visit(const TiledMapCompatibleCollider2D* tile, const BoxCollider2D* other) override;

	virtual bool Visit(const TiledMapCompatibleCollider2D* tile, const CircleCollider2D* other) override;
};