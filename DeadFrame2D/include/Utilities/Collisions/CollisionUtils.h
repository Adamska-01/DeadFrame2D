#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"


struct SDL_Rect;
struct SDL_FRect;

namespace DF2D::Core
{ 
	struct Circle;
}


namespace DF2D::Utilities::Collision
{
	DF2D_API bool PointVsBox(const Core::Vector2F& point, const SDL_FRect& box);

	DF2D_API bool PointVsCircle(const Core::Vector2F& point, const Core::Circle& circle);

	DF2D_API bool RectVsRect(const SDL_FRect& boxA, const SDL_FRect& boxB);

	DF2D_API bool RectVsRect(const SDL_Rect& boxA, const SDL_Rect& boxB);

	DF2D_API bool CircleVsRect(const Core::Circle& circle, const SDL_FRect& rect);

	DF2D_API bool SegmentVsRect(const Core::Vector2F& p0, const Core::Vector2F& p1, const SDL_FRect& rect);

	DF2D_API bool RayVsRect(
		const Core::Vector2F& ray_origin,
		const Core::Vector2F& ray_dir,
		const SDL_Rect& target,
		Core::Vector2F& contact_point,
		Core::Vector2F& contact_normal,
		float& t_hit_near);
}