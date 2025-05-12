#pragma once
#include "Data/Physics/BodyType2D.h"
#include <Math/Vector2.h>


/**
 * @brief Wrapper for Box2D body definition with default values matching b2BodyDef.
 */
class BodyDefinition2D
{
public:
	Vector2F position = Vector2F(0.0f, 0.0f);

	Vector2F linearVelocity = Vector2F(0.0f, 0.0f);

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