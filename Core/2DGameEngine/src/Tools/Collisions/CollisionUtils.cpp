#include "Components/Generic/Circle.h"
#include "Tools/Collisions/CollisionUtils.h"
#include <SDL.h>


SDL_Rect CollisionUtils::GetExpandedTarget(const SDL_Rect* colliderRect, const SDL_Rect* tileRect)
{
	return SDL_Rect 
	{
		tileRect->x - colliderRect->w / 2,
		tileRect->y - colliderRect->h / 2,
		tileRect->w + colliderRect->w,
		tileRect->h + colliderRect->h
	};
}

bool CollisionUtils::PointVsBox(const Vector2F& point, const SDL_Rect* box)
{
	return point.x >= box->x 
		&& point.y >= box->y 
		&& point.x < box->x + box->w 
		&& point.y < box->y + box->h;
}

bool CollisionUtils::PointVsCircle(const Vector2F& point, const Circle* circle)
{
	auto dx = point.x - circle->position.x;
	auto dy = point.y - circle->position.y;

	return (dx * dx + dy * dy) <= (circle->radius * circle->radius);
}

bool CollisionUtils::RectVsRect(const SDL_Rect* boxA, const SDL_Rect* boxB)
{
	return boxA->x < boxB->x + boxB->w 
		&& boxB->x < boxA->x + boxA->w
		&& boxA->y < boxB->y + boxB->h 
		&& boxB->y < boxA->y + boxA->h;
}

bool CollisionUtils::RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const SDL_Rect* target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near)
{
	contact_normal = { 0.0f, 0.0f };
	contact_point = { 0.0f, 0.0f };

	// Cache division
	auto invdir = Vector2F(1.0f / ray_dir.x, 1.0f / ray_dir.y);

	// Calculate intersections with rectangle bounding axes
	auto t_near = (Vector2F(target->x, target->y) - ray_origin) * invdir;
	auto t_far = (Vector2F(target->x, target->y) + Vector2F(target->w, target->h) - ray_origin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x) || std::isnan(t_near.y) || std::isnan(t_near.x))
		return false;

	// Sort distances
	if (t_near.x > t_far.x)
	{
		std::swap(t_near.x, t_far.x);
	}
	
	if (t_near.y > t_far.y)
	{
		std::swap(t_near.y, t_far.y);
	}

	// Early rejection
	if (t_near.x > t_far.y || t_near.y > t_far.x) 
		return false;

	// Closest 'time' will be the first contact
	t_hit_near = std::max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	auto t_hit_far = std::min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	contact_point = ray_origin + ray_dir * t_hit_near;

	if (t_near.x > t_near.y)
	{
		contact_normal = invdir.x < 0 ? Vector2F(1, 0) : Vector2F(-1, 0);
	}
	else if (t_near.x < t_near.y)
	{
		contact_normal = invdir.y < 0 ? Vector2F(0,  1) : Vector2F(0, -1);
	}

	// Note if t_near == t_far, collision is principly in a diagonal
	// so pointless to resolve. By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}