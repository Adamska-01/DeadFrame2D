#include "Constants/MathConstants.h"
#include "Utilities/Collisions/CollisionUtils.h"
#include <algorithm>
#include <array>


namespace DF2D::Utilities::Collision
{
	using namespace DF2D::Constants;
	using namespace DF2D::Core;


	bool PointVsBox(const Vector2F& point, const RectF& box)
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

	bool RectVsRect(const RectF& boxA, const RectF& boxB)
	{
		return boxA.x < boxB.x + boxB.w
			&& boxB.x < boxA.x + boxA.w
			&& boxA.y < boxB.y + boxB.h
			&& boxB.y < boxA.y + boxA.h;
	}

	bool RectVsRect(const RectI& boxA, const RectI& boxB)
	{
		return boxA.x < boxB.x + boxB.w
			&& boxB.x < boxA.x + boxA.w
			&& boxA.y < boxB.y + boxB.h
			&& boxB.y < boxA.y + boxA.h;
	}

	bool CircleVsRect(const Circle& circle, const RectF& rect)
	{
		auto closestX = std::clamp(circle.position.x, rect.x, rect.x + rect.w);
		auto closestY = std::clamp(circle.position.y, rect.y, rect.y + rect.h);

		auto dx = circle.position.x - closestX;
		auto dy = circle.position.y - closestY;

		return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
	}

	bool SegmentVsRect(const Vector2F& p0, const Vector2F& p1, const RectF& rect)
	{
		auto minX = std::min(p0.x, p1.x);
		auto maxX = std::max(p0.x, p1.x);
		auto minY = std::min(p0.y, p1.y);
		auto maxY = std::max(p0.y, p1.y);

		if (maxX < rect.x
			|| minX > rect.x + rect.w
			|| maxY < rect.y
			|| minY > rect.y + rect.h)
			return false;

		auto dx = p1.x - p0.x;
		auto dy = p1.y - p0.y;

		std::array<float, 4> p = { -dx, dx, -dy, dy };
		std::array<float, 4> q = { p0.x - rect.x, (rect.x + rect.w) - p0.x,
								   p0.y - rect.y, (rect.y + rect.h) - p0.y };

		auto u1 = 0.0f;
		auto u2 = 1.0f;

		for (auto i = 0; i < 4; ++i)
		{
			if (p[i] == 0.0f)
			{
				if (q[i] < MathConstants::EPSILON)
					return false;

				continue;
			}

			auto u = q[i] / p[i];
			if (p[i] < 0.0f)
			{
				u1 = std::max(u1, u);

				if (u1 > u2)
					return false;
			}
			else
			{
				u2 = std::min(u2, u);

				if (u2 < u1)
					return false;
			}
		}

		return true;
	}

	bool RayVsRect(const Vector2F& ray_origin, const Vector2F& ray_dir, const RectI& target, Vector2F& contact_point, Vector2F& contact_normal, float& t_hit_near)
	{
		contact_normal = { 0.0f, 0.0f };
		contact_point = { 0.0f, 0.0f };

		auto invdir = Vector2F(1.0f / ray_dir.x, 1.0f / ray_dir.y);

		auto t_near = (Vector2F(static_cast<float>(target.x), static_cast<float>(target.y)) - ray_origin) * invdir;
		auto t_far = (Vector2F(static_cast<float>(target.x + target.w), static_cast<float>(target.y + target.h)) - ray_origin) * invdir;

		if (std::isnan(t_far.y) || std::isnan(t_far.x) || std::isnan(t_near.y) || std::isnan(t_near.x))
			return false;

		if (t_near.x > t_far.x)
		{
			std::swap(t_near.x, t_far.x);
		}

		if (t_near.y > t_far.y)
		{
			std::swap(t_near.y, t_far.y);
		}

		if (t_near.x > t_far.y || t_near.y > t_far.x)
			return false;

		t_hit_near = std::max(t_near.x, t_near.y);

		auto t_hit_far = std::min(t_far.x, t_far.y);

		if (t_hit_far < 0)
			return false;

		contact_point = ray_origin + ray_dir * t_hit_near;

		if (t_near.x > t_near.y)
		{
			contact_normal = invdir.x < 0 ? Vector2F::Right : Vector2F::Left;
		}
		else if (t_near.x < t_near.y)
		{
			contact_normal = invdir.y < 0 ? Vector2F::Down : Vector2F::Up;
		}

		return true;
	}
}