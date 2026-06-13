#pragma once
#include "Core/Math/Rect.h"
#include "Core/Math/Circle.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"


namespace DF2D::Utilities::Collision
{
	DF2D_API bool PointVsBox(const Core::Vector2F& point, const Core::RectF& box);

	DF2D_API bool PointVsCircle(const Core::Vector2F& point, const Core::Circle& circle);

	DF2D_API bool RectVsRect(const Core::RectF& boxA, const Core::RectF& boxB);

	DF2D_API bool RectVsRect(const Core::RectI& boxA, const Core::RectI& boxB);

	DF2D_API bool CircleVsRect(const Core::Circle& circle, const Core::RectF& rect);

	DF2D_API bool SegmentVsRect(const Core::Vector2F& p0, const Core::Vector2F& p1, const Core::RectF& rect);

	DF2D_API bool RayVsRect(
		const Core::Vector2F& ray_origin,
		const Core::Vector2F& ray_dir,
		const Core::RectI& target,
		Core::Vector2F& contact_point,
		Core::Vector2F& contact_normal,
		float& t_hit_near);
}