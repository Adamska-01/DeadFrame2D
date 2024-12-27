#include <Components/Collisions/Circle.h>
#include <Components/Collisions/CollisionHandler.h>
#include <Math/Vector2.h>
#include <SDL.h>


bool CollisionHandler::BoxToBoxCollision(const SDL_Rect& A, const SDL_Rect& B)
{
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

bool CollisionHandler::CircleToBoxCollision(const Circle& A, const SDL_Rect& B, Vector2& collisionPoint)
{
	// Closest points on collision box
	Vector2 closestPoint;

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

	collisionPoint = closestPoint;

	// Check if the point is inside the circle
	if (A.position.Distance(closestPoint) < A.radius)
		return true;

	// No collision
	return false; 
}

bool CollisionHandler::CircleToCircleCollision(const Circle& A, const Circle& B)
{
	// Check if the point is touching the circle
	if (A.position.Distance(B.position) <= A.radius + B.radius)
		return true;

	// No collision
	return false; 
}

bool CollisionHandler::PointInCircle(const Vector2& point, const Circle& circle)
{
	auto dx = point.x - circle.position.x;
	auto dy = point.y - circle.position.y;

	return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}