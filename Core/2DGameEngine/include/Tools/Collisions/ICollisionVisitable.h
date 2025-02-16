#pragma once


class Collider2D;
class BoxCollider2D;
class CircleCollider2D;
class TiledMapCompatibleCollider2D;
class ICollisionVisitor;


class ICollisionVisitable
{
public:
	virtual bool Accept(ICollisionVisitor& visitor, Collider2D* other) = 0;


	virtual bool AcceptDispatch(BoxCollider2D* other, ICollisionVisitor& visitor)
	{
		return false;
	};

	virtual bool AcceptDispatch(CircleCollider2D* other, ICollisionVisitor& visitor)
	{
		return false;
	};

	virtual bool AcceptDispatch(TiledMapCompatibleCollider2D* other, ICollisionVisitor& visitor)
	{
		return false;
	};
};