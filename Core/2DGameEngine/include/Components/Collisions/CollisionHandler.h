#pragma once
#include "Math/Vector2.h"


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
	
	static bool CircleToBoxCollision(const Circle& A, const SDL_Rect& B, Vector2F& P);
	
	static bool CircleToCircleCollision(const Circle& A, const Circle& B);

	static bool PointInCircle(const Vector2F& point, const Circle& circle);
};
