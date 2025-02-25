#pragma once
#include "Math/Vector2.h"
#include "Tools/Collisions/ICollisionVisitor.h"


struct Circle;
struct SDL_Rect;


class CollisionHandler : public ICollisionVisitor
{
public:
	CollisionHandler() = default;

	~CollisionHandler() = default;


	static bool PointVsBox(const Vector2F& point, const SDL_Rect* box);

	static bool PointVsCircle(const Vector2F& point, const Circle* circle);

	static bool RectVsRect(const SDL_Rect* boxA, const SDL_Rect* boxB);
	
	
	virtual bool Visit(BoxCollider2D* box, BoxCollider2D* other) override;

	virtual bool Visit(BoxCollider2D* box, CircleCollider2D* other) override;

	virtual bool Visit(BoxCollider2D* box, TiledMapCompatibleCollider2D* other) override;


	virtual bool Visit(CircleCollider2D* circle, BoxCollider2D* other) override;

	virtual bool Visit(CircleCollider2D* circle, CircleCollider2D* other) override;

	virtual bool Visit(CircleCollider2D* circle, TiledMapCompatibleCollider2D* other) override;


	virtual bool Visit(TiledMapCompatibleCollider2D* tile, BoxCollider2D* other) override;

	virtual bool Visit(TiledMapCompatibleCollider2D* tile, CircleCollider2D* other) override;
};