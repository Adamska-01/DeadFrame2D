#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Components/Physics/BodyType2D.h"


namespace DF2D::Data
{
	/**
	 * @brief Wrapper for Box2D body definition with default values matching b2BodyDef.
	 */
	struct BodyDefinition2D
	{
		Core::Vector2F position = Core::Vector2F(0.0f, 0.0f);

		Core::Vector2F linearVelocity = Core::Vector2F(0.0f, 0.0f);

		BodyType2D type = BodyType2D::Static;

		float angle = 0.0f;

		float angularVelocity = 0.0f;

		float linearDamping = 0.0f;

		float angularDamping = 0.0f;

		float gravityScale = 1.0f;

		bool allowSleep = true;

		bool awake = true;

		bool fixedRotation = false;

		bool bullet = false;

		bool enabled = true;
	};
}