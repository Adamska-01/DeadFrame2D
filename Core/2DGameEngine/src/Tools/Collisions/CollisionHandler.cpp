#include "Components/Collisions/BoxCollider2D.h"
#include "Components/Collisions/CircleCollider2D.h"
#include "Components/Collisions/Tile/Tiled/TiledMapCompatibleCollider2D.h"
#include "Tools/Collisions/CollisionHandler.h"


bool CollisionHandler::PointInCircle(const Vector2F& point, const CircleCollider2D& collider)
{
	auto circle = collider.GetCircle();

	auto dx = point.x - circle.position.x;
	auto dy = point.y - circle.position.y;

	return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

bool CollisionHandler::Visit(BoxCollider2D& box, Collider2D& other)
{
	return other.Accept(*this, box);
}

bool CollisionHandler::Visit(CircleCollider2D& circle, Collider2D& other)
{
	return other.Accept(*this, circle);
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D& tile, Collider2D& other)
{
	return other.Accept(*this, tile);
}

bool CollisionHandler::Visit(BoxCollider2D& box, BoxCollider2D& other)
{
	auto A = box.GetCollisionBox();
	auto B = other.GetCollisionBox();

	// Sides of the rectangle 
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;

	// Sides of rect A
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	// Sides of rect B
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	// Check collisions
	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;

	// Collision happened
	return true;
}

bool CollisionHandler::Visit(BoxCollider2D& box, CircleCollider2D& other)
{
	auto A = other.GetCircle();
	auto B = box.GetCollisionBox();

	// Closest points on collision box
	Vector2F closestPoint;

	// Find closest x offset
	if (A.position.x < B.x)
		closestPoint.x = B.x;
	else if (A.position.x > B.x + B.w)
		closestPoint.x = B.x + B.w;
	else
		closestPoint.x = A.position.x;

	// Find closest y offset
	if (A.position.y < B.y)
		closestPoint.y = B.y;
	else if (A.position.y > B.y + B.h)
		closestPoint.y = B.y + B.h;
	else
		closestPoint.y = A.position.y;

	//collisionPoint = closestPoint;

	// Check if the point is inside the circle
	if (A.position.Distance(closestPoint) < A.radius)
		return true;

	// No collision
	return false;
}

bool CollisionHandler::Visit(BoxCollider2D& box, TiledMapCompatibleCollider2D& other)
{
	auto colliderRect = box.GetCollisionBox();

	// Replace these with the real values
	auto tilesize = 32;
	auto rowCount = 20;
	auto colCount = 60;

	auto left_tile = colliderRect.x / tilesize;
	auto right_tile = (colliderRect.x + colliderRect.w) / tilesize;

	auto top_tile = colliderRect.y / tilesize;
	auto bottom_tile = (colliderRect.y + colliderRect.h) / tilesize;

	if (top_tile < 0)
		top_tile = 0;

	if (right_tile > colCount)
		right_tile = colCount;

	if (top_tile < 0)
		top_tile = 0;

	if (bottom_tile > rowCount)
		bottom_tile = rowCount;

	for (auto i = left_tile; i <= right_tile; i++)
	{
		for (auto j = top_tile; j <= bottom_tile; j++)
		{
			auto withinBounds = i >= 0 && i < colCount && j >= 0 && j < rowCount;
			//auto isSolidTile = collisionTileMap[j][i] > 0;

			//if (withinBounds && isSolidTile)
			//	return true;
		}
	}

	return false;
}

bool CollisionHandler::Visit(CircleCollider2D& circle, BoxCollider2D& other)
{
	return Visit(other, circle);
}

bool CollisionHandler::Visit(CircleCollider2D& circle, CircleCollider2D& other)
{
	auto A = circle.GetCircle();
	auto B = other.GetCircle();

	// Check if the point is touching the circle
	if (A.position.Distance(B.position) <= A.radius + B.radius)
		return true;

	// No collision
	return false;
}

bool CollisionHandler::Visit(CircleCollider2D& circle, TiledMapCompatibleCollider2D& other)
{
	return false;
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D& tile, BoxCollider2D& other)
{
	return Visit(other, tile);
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D& tile, CircleCollider2D& other)
{
	return Visit(other, tile);
}

bool CollisionHandler::Visit(TiledMapCompatibleCollider2D& tile, TiledMapCompatibleCollider2D& other)
{
	return false;
}
