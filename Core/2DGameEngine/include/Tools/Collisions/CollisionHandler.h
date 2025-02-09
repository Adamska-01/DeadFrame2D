#pragma once
#include "Tools/Collisions/ColliderVisitor.h"
#include "Math/Vector2.h"


class CollisionHandler : public ColliderVisitor
{
private:
	CollisionHandler() = default;

	~CollisionHandler() = default;

	CollisionHandler(const CollisionHandler&) = delete;

	CollisionHandler(CollisionHandler&&) = delete;


	CollisionHandler& operator=(const CollisionHandler&) = delete;

	CollisionHandler& operator=(CollisionHandler&&) = delete;


	static bool PointInCircle(const Vector2F& point, const CircleCollider2D& circle);


public:
	virtual bool Visit(BoxCollider2D& box, Collider2D& other) override;

	virtual bool Visit(CircleCollider2D& circle, Collider2D& other) override;
	
	virtual bool Visit(TiledMapCompatibleCollider2D& tile, Collider2D& other) override;

	
	virtual bool Visit(BoxCollider2D& box, BoxCollider2D& other) override;
	
	virtual bool Visit(BoxCollider2D& box, CircleCollider2D& other) override;
	
	virtual bool Visit(BoxCollider2D& box, TiledMapCompatibleCollider2D& other) override;

	
	virtual bool Visit(CircleCollider2D& circle, BoxCollider2D& other) override;
	
	virtual bool Visit(CircleCollider2D& circle, CircleCollider2D& other) override;
	
	virtual bool Visit(CircleCollider2D& circle, TiledMapCompatibleCollider2D& other) override;


	virtual bool Visit(TiledMapCompatibleCollider2D& tile, BoxCollider2D& other) override;
	
	virtual bool Visit(TiledMapCompatibleCollider2D& tile, CircleCollider2D& other) override;

	virtual bool Visit(TiledMapCompatibleCollider2D& tile, TiledMapCompatibleCollider2D& other) override;
};