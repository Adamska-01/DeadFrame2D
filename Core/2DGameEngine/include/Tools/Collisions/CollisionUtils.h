#pragma once
#include "Math/Vector2.h"


struct Circle;
struct SDL_Rect;


class CollisionUtils
{
private:
	SDL_Rect GetExpandedTarget(const SDL_Rect* colliderRect, const SDL_Rect* tileRect);


public:
	CollisionUtils() = default;

	~CollisionUtils() = default;


	static bool PointVsBox(const Vector2F& point, const SDL_Rect* box);

	static bool PointVsCircle(const Vector2F& point, const Circle* circle);

	static bool RectVsRect(const SDL_Rect* boxA, const SDL_Rect* boxB);

	static bool RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const SDL_Rect* target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near);
};