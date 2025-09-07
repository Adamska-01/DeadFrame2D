#pragma once
#include "Core/Math/Vector2.h"


struct Circle;
struct SDL_Rect;


// TODO: Delete 'GetExpandedTarget' and repplace the class with a namespace called "CollisionUtils" or something, since it has no state.
// TODO: replace all the "const T*" with "const T&"
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

	static bool CircleVsRect(const Circle* circle, const SDL_Rect* rect);

	static bool SegmentVsRect(const Vector2F& p0, const Vector2F& p1, const SDL_Rect* rect);

	static bool RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const SDL_Rect* target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near);
};