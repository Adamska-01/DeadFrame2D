#pragma once


class Vector2;
struct Circle;
struct SDL_Rect;


class CollisionHandler
{
private:
	CollisionHandler() = delete;
	
	CollisionHandler(const CollisionHandler& other) = delete;

	~CollisionHandler() = delete;


public:
	static bool BoxToBoxCollision(const SDL_Rect& A, const SDL_Rect& B);
	
	static bool CircleToBoxCollision(const Circle& A, const SDL_Rect& B, Vector2& P);
	
	static bool CircleToCircleCollision(const Circle& A, const Circle& B);

	static bool PointInCircle(const Vector2& point, const Circle& circle);
};
