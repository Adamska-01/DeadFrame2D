#pragma once
#include "Constants/PhysicsConstants.h"
#include "Core/Math/Vector2.h"
#include <box2d/box2d.h>


namespace DeadFrame2D::Utilities
{
	// TODO: Put these in a static class

	/**
	 * @brief Creates a new b2PolygonShape configured as a box.
	 *
	 * @param halfWidth  Half-width of the box in pixels.
	 * @param halfHeight Half-height of the box in pixels.
	 * @param center     Center position of the box in pixels (optional).
	 * @param angle      Rotation angle of the box in radians (optional).
	 * @return b2PolygonShape* Pointer to the newly created shape. Caller is responsible for deleting it.
	 */
	inline b2PolygonShape* CreateBoxShape(
		float halfWidth,
		float halfHeight,
		const DeadFrame2D::Core::Vector2F& center = DeadFrame2D::Core::Vector2F::Zero,
		float angle = 0.0f)
	{
		auto boxShape = new b2PolygonShape();

		const auto PIXEL_TO_METER = Shared::Constants::Physics::PIXEL_TO_METER;

		boxShape->SetAsBox(
			halfWidth * PIXEL_TO_METER,
			halfHeight * PIXEL_TO_METER,
			b2Vec2(
				center.x * PIXEL_TO_METER,
				center.y * PIXEL_TO_METER),
			angle);
		
		return boxShape;
	}

	/**
	* @brief Creates a new b2CircleShape with the specified radius and center.
	*
	* @param radius Radius of the circle in pixels.
	* @param center Center of the circle in pixels (optional).
	* @return b2CircleShape* Pointer to the newly created shape. Caller is responsible for deleting it.
	*/
	inline b2CircleShape* CreateCircleShape(float radius, const DeadFrame2D::Core::Vector2F& center = DeadFrame2D::Core::Vector2F::Zero)
	{
		auto circleShape = new b2CircleShape();

		const auto PIXEL_TO_METER = Shared::Constants::Physics::PIXEL_TO_METER;

		circleShape->m_radius = radius * PIXEL_TO_METER;
		circleShape->m_p = b2Vec2(
			center.x * PIXEL_TO_METER,
			center.y * PIXEL_TO_METER);
		
		return circleShape;
	}
}