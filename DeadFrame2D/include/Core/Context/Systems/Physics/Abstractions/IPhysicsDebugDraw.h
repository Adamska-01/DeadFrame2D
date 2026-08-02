#pragma once
#include "Core/Math/Color.h"
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Receives debug-draw primitives from a physics backend, expressed in engine types only (pixels).
	 */
	class DF2D_API IPhysicsDebugDraw
	{
	public:
		virtual ~IPhysicsDebugDraw() = default;


		virtual void DrawSegment(const Vector2F& p1, const Vector2F& p2, const Color& color) = 0;

		virtual void DrawCircle(const Vector2F& center, float radius, bool filled, const Color& color) = 0;

		virtual void DrawPoint(const Vector2F& position, float size, const Color& color) = 0;
	};
}