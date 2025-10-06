#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"


struct SDL_Rect;

namespace DeadFrame2D::Core
{ 
	struct Circle;
}


namespace DeadFrame2D::Utilities::Collision
{
	// TODO: Delete 'GetExpandedTarget' and repplace the class with a namespace called "CollisionUtils" or something, since it has no state.
	// TODO: replace all the "const T*" with "const T&"
	DF2D_API bool PointVsBox(const DeadFrame2D::Core::Vector2F& point, const SDL_Rect* box);

	DF2D_API bool PointVsCircle(const DeadFrame2D::Core::Vector2F& point, const DeadFrame2D::Core::Circle* circle);

	DF2D_API bool RectVsRect(const SDL_Rect* boxA, const SDL_Rect* boxB);

	DF2D_API bool CircleVsRect(const DeadFrame2D::Core::Circle* circle, const SDL_Rect* rect);

	DF2D_API bool SegmentVsRect(const DeadFrame2D::Core::Vector2F& p0, const DeadFrame2D::Core::Vector2F& p1, const SDL_Rect* rect);

	DF2D_API bool RayVsRect(
		const DeadFrame2D::Core::Vector2F& ray_origin,
		const DeadFrame2D::Core::Vector2F& ray_dir,
		const SDL_Rect* target,
		DeadFrame2D::Core::Vector2F& contact_point,
		DeadFrame2D::Core::Vector2F& contact_normal,
		float& t_hit_near);
}