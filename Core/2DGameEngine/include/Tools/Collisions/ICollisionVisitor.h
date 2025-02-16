#pragma once


class Collider2D;
class BoxCollider2D;
class CircleCollider2D;
class TiledMapCompatibleCollider2D;


class ICollisionVisitor
{
public:
	virtual bool Visit(const BoxCollider2D* box, const BoxCollider2D* other) = 0;

	virtual bool Visit(const BoxCollider2D* box, const CircleCollider2D* other) = 0;

	virtual bool Visit(const BoxCollider2D* box, const TiledMapCompatibleCollider2D* other) = 0;


	virtual bool Visit(const CircleCollider2D* circle, const BoxCollider2D* other) = 0;

	virtual bool Visit(const CircleCollider2D* circle, const CircleCollider2D* other) = 0;

	virtual bool Visit(const CircleCollider2D* circle, const TiledMapCompatibleCollider2D* other) = 0;


	virtual bool Visit(const TiledMapCompatibleCollider2D* tile, const BoxCollider2D* other) = 0;

	virtual bool Visit(const TiledMapCompatibleCollider2D* tile, const CircleCollider2D* other) = 0;
};