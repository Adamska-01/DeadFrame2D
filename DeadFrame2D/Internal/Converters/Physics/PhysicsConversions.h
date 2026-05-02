#pragma once
#include "Data/Components/Collision/PhysicsMaterial.h"
#include "Data/Components/Physics/BodyDefinition2D.h"
#include "Data/Components/Physics/BodyType2D.h"
#include <box2d/b2_body.h>
#include <cassert>


namespace DF2D::Internal::Physics
{
	/**
	 * @brief Converts a custom BodyType2D to Box2D's b2BodyType.
	 * @param type The custom BodyType2D value.
	 * @return Corresponding b2BodyType.
	 */
	inline b2BodyType ToB2BodyType(Data::BodyType2D type)
	{
		switch (type)
		{
		case Data::BodyType2D::Static:
			return b2_staticBody;

		case Data::BodyType2D::Kinematic:
			return b2_kinematicBody;

		case Data::BodyType2D::Dynamic:
			return b2_dynamicBody;

		default:
			return b2_staticBody;
		}
	}

	/**
	 * @brief Converts BodyDefinition2D wrapper into a b2BodyDef instance for use with Box2D.
	 */
	inline b2BodyDef ToB2BodyDef(const Data::BodyDefinition2D& bodyDef)
	{
		b2BodyDef b2Def;

		b2Def.position = b2Vec2(bodyDef.position.x, bodyDef.position.y);
		b2Def.angle = bodyDef.angle;
		b2Def.linearVelocity = b2Vec2(bodyDef.linearVelocity.x, bodyDef.linearVelocity.y);
		b2Def.angularVelocity = bodyDef.angularVelocity;
		b2Def.linearDamping = bodyDef.linearDamping;
		b2Def.angularDamping = bodyDef.angularDamping;
		b2Def.allowSleep = bodyDef.allowSleep;
		b2Def.awake = bodyDef.awake;
		b2Def.fixedRotation = bodyDef.fixedRotation;
		b2Def.bullet = bodyDef.bullet;
		b2Def.type = ToB2BodyType(bodyDef.type);
		b2Def.enabled = bodyDef.enabled;
		b2Def.gravityScale = bodyDef.gravityScale;

		return b2Def;
	}
	
	/**
	 * @brief Converts PhysicsMaterial wrapper into a b2FixtureDef instance for use with Box2D.
	 */
	inline b2FixtureDef ToB2FixtureDef(const Data::PhysicsMaterial& physicsMaterial, uintptr_t userDataPtr)
	{
		b2FixtureDef b2FDef;

		b2FDef.shape = physicsMaterial.shape;
		b2FDef.friction = physicsMaterial.friction;
		b2FDef.restitution = physicsMaterial.restitution;
		b2FDef.restitutionThreshold = physicsMaterial.restitutionThreshold;
		b2FDef.density = physicsMaterial.density;
		b2FDef.isSensor = physicsMaterial.isSensor;
		b2FDef.filter.categoryBits = physicsMaterial.filter.categoryBits;
		b2FDef.filter.groupIndex = physicsMaterial.filter.groupIndex;
		b2FDef.filter.maskBits = physicsMaterial.filter.maskBits;
		b2FDef.userData.pointer = userDataPtr;

		assert(b2FDef.shape != nullptr && "Shape must not be null");

		return b2FDef;
	}


	/**
	 * @brief Creates a new b2PolygonShape configured as a box.
	 *
	 * @param halfWidth  Half-width of the box in pixels.
	 * @param halfHeight Half-height of the box in pixels.
	 * @param center     Center position of the box in pixels (optional).
	 * @param angle      Rotation angle of the box in radians (optional).
	 * @return b2PolygonShape* Pointer to the newly created shape. Caller is responsible for deleting it.
	 */
	inline b2PolygonShape* ToB2BoxShape(
		float halfWidth,
		float halfHeight,
		const Core::Vector2F& center = Core::Vector2F::Zero,
		float angle = 0.0f)
	{
		auto boxShape = new b2PolygonShape();

		const auto METER_PER_PIXEL = Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		boxShape->SetAsBox(
			halfWidth * METER_PER_PIXEL,
			halfHeight * METER_PER_PIXEL,
			b2Vec2(
				center.x * METER_PER_PIXEL,
				center.y * METER_PER_PIXEL),
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
	inline b2CircleShape* ToB2CircleShape(float radius, const Core::Vector2F& center = Core::Vector2F::Zero)
	{
		auto circleShape = new b2CircleShape();

		const auto METER_PER_PIXEL = Core::PhysicsEngine2D::GetPhysicsConfig().meterPerPixel;

		circleShape->m_radius = radius * METER_PER_PIXEL;
		circleShape->m_p = b2Vec2(
			center.x * METER_PER_PIXEL,
			center.y * METER_PER_PIXEL);

		return circleShape;
	}
}