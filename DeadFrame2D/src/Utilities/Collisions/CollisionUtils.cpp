#include "Core/Math/Circle.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <algorithm>
#include <SDL.h>


namespace DeadFrame2D::Utilities::Collision
{
	using namespace DeadFrame2D::Core;


	bool PointVsBox(const Vector2F& point, const SDL_Rect& box)
	{
		return point.x >= box.x 
			&& point.y >= box.y 
			&& point.x < box.x + box.w 
			&& point.y < box.y + box.h;
	}

	bool PointVsCircle(const Vector2F& point, const Circle& circle)
	{
		auto dx = point.x - circle.position.x;
		auto dy = point.y - circle.position.y;

		return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
	}

	bool RectVsRect(const SDL_Rect& boxA, const SDL_Rect& boxB)
	{
		return boxA.x < boxB.x + boxB.w 
			&& boxB.x < boxA.x + boxA.w
			&& boxA.y < boxB.y + boxB.h 
			&& boxB.y < boxA.y + boxA.h;
	}

	bool CircleVsRect(const Circle& circle, const SDL_Rect& rect)
	{
		// Clamp circle center to the closest point on the rectangle
		auto closestX = std::clamp(circle.position.x, static_cast<float>(rect.x), static_cast<float>(rect.x + rect.w));
		auto closestY = std::clamp(circle.position.y, static_cast<float>(rect.y), static_cast<float>(rect.y + rect.h));

		// Compute distance from circle center to closest point
		auto dx = circle.position.x - closestX;
		auto dy = circle.position.y - closestY;

		// Check if distance is less than or equal to radius squared
		return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
	}

	bool SegmentVsRect(const Vector2F& p0, const Vector2F& p1, const SDL_Rect& rect)
	{
		// Compute the min and max of the segment
		auto minX = std::min(p0.x, p1.x);
		auto maxX = std::max(p0.x, p1.x);
		auto minY = std::min(p0.y, p1.y);
		auto maxY = std::max(p0.y, p1.y);

		// Early AABB rejection
		if (maxX < rect.x 
			|| minX > rect.x + rect.w 
			|| maxY < rect.y 
			|| minY > rect.y + rect.h)
		{
			return false;
		}

		// Liang-Barsky line clipping algorithm
		auto dx = p1.x - p0.x;
		auto dy = p1.y - p0.y;

		auto p = { -dx,  dx, -dy,  dy };
		auto q = { p0.x - rect.x, (rect.x + rect.w) - p0.x, p0.y - rect.y, (rect.y + rect.h) - p0.y };

		float u1 = 0.0f;
		float u2 = 1.0f;

		auto itP = p.begin();
		auto itQ = q.begin();

		for (auto i = 0; i < 4; ++i, ++itP, ++itQ)
		{
			auto pi = *itP;
			auto qi = *itQ;

			if (pi == 0.0f)
			{
				// Line is parallel and outside the rect
				if (qi < 0.0f)
					return false;
			
				continue;
			}

			auto u = qi / pi;
			if (pi < 0.0f)
			{
				if (u > u2)
					return false;

				if (u > u1)
					u1 = u;
			}
			else
			{
				if (u < u1)
					return false;

				if (u < u2)
					u2 = u;
			}
		}

		return true;
	}

	bool RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const SDL_Rect& target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near)
	{
		contact_normal = { 0.0f, 0.0f };
		contact_point = { 0.0f, 0.0f };

		// Cache division
		auto invdir = Vector2F(1.0f / ray_dir.x, 1.0f / ray_dir.y);

		// Calculate intersections with rectangle bounding axes
		auto t_near = (Vector2F(static_cast<float>(target.x), static_cast<float>(target.y)) - ray_origin) * invdir;
		auto t_far = (Vector2F(static_cast<float>(target.x + target.w), static_cast<float>(target.y + target.h)) - ray_origin) * invdir;

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
			contact_normal = invdir.x < 0 ? Vector2F::Right : Vector2F::Left;
		}
		else if (t_near.x < t_near.y)
		{
			contact_normal = invdir.y < 0 ? Vector2F::Down : Vector2F::Up;
		}

		// Note if t_near == t_far, collision is principly in a diagonal
		// so pointless to resolve. By returning a CN={0,0} even though its
		// considered a hit, the resolver wont change anything.
		return true;
	}
}