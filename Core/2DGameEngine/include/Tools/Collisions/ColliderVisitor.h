#pragma once


class Collider2D;
class BoxCollider2D;
class CircleCollider2D;
class TiledMapCompatibleCollider2D;


class ColliderVisitor 
{
public:
	virtual bool Visit(BoxCollider2D& box, Collider2D& other) = 0;

	virtual bool Visit(CircleCollider2D& circle, Collider2D& other) = 0;

	virtual bool Visit(TiledMapCompatibleCollider2D& tile, Collider2D& other) = 0;


	virtual bool Visit(BoxCollider2D& box, BoxCollider2D& other) = 0;

	virtual bool Visit(BoxCollider2D& box, CircleCollider2D& other) = 0;

	virtual bool Visit(BoxCollider2D& box, TiledMapCompatibleCollider2D& other) = 0;


	virtual bool Visit(CircleCollider2D& circle, BoxCollider2D& other) = 0;

	virtual bool Visit(CircleCollider2D& circle, CircleCollider2D& other) = 0;

	virtual bool Visit(CircleCollider2D& circle, TiledMapCompatibleCollider2D& other) = 0;


	virtual bool Visit(TiledMapCompatibleCollider2D& tile, BoxCollider2D& other) = 0;

	virtual bool Visit(TiledMapCompatibleCollider2D& tile, CircleCollider2D& other) = 0;

	virtual bool Visit(TiledMapCompatibleCollider2D& tile, TiledMapCompatibleCollider2D& other) = 0;
};