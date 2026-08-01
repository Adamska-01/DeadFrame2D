#pragma once
#include "Data/Components/Collision/PhysicsMaterial.h"
#include "Data/Components/Physics/BodyDefinition2D.h"
#include "Data/Components/Physics/BodyType2D.h"
#include <box2d/box2d.h>


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
	 * @param bodyDef The engine body definition (position in pixels).
	 * @param meterPerPixel Conversion factor from pixels to meters.
	 */
	inline b2BodyDef ToB2BodyDef(const Data::BodyDefinition2D& bodyDef, float meterPerPixel)
	{
		b2BodyDef b2Def;

		b2Def.position = b2Vec2(bodyDef.position.x * meterPerPixel, bodyDef.position.y * meterPerPixel);
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
	 *
	 * The shape and user data are not set here — the backend owns both.
	 */
	inline b2FixtureDef ToB2FixtureDef(const Data::PhysicsMaterial& physicsMaterial)
	{
		b2FixtureDef b2FDef;

		b2FDef.friction = physicsMaterial.friction;
		b2FDef.restitution = physicsMaterial.restitution;
		b2FDef.restitutionThreshold = physicsMaterial.restitutionThreshold;
		b2FDef.density = physicsMaterial.density;
		b2FDef.isSensor = physicsMaterial.isSensor;
		b2FDef.filter.categoryBits = physicsMaterial.filter.categoryBits;
		b2FDef.filter.groupIndex = physicsMaterial.filter.groupIndex;
		b2FDef.filter.maskBits = physicsMaterial.filter.maskBits;

		return b2FDef;
	}

	/**
	 * @brief Creates a b2PolygonShape from an engine box shape definition (pixels).
	 */
	inline b2PolygonShape ToB2BoxShape(const Data::BoxShapeDefinition2D& shapeDefinition, float meterPerPixel)
	{
		b2PolygonShape boxShape;

		boxShape.SetAsBox(
			shapeDefinition.halfExtents.x * meterPerPixel,
			shapeDefinition.halfExtents.y * meterPerPixel,
			b2Vec2(
				shapeDefinition.center.x * meterPerPixel,
				shapeDefinition.center.y * meterPerPixel),
			shapeDefinition.angle);

		return boxShape;
	}

	/**
	 * @brief Creates a b2CircleShape from an engine circle shape definition (pixels).
	 */
	inline b2CircleShape ToB2CircleShape(const Data::CircleShapeDefinition2D& shapeDefinition, float meterPerPixel)
	{
		b2CircleShape circleShape;

		circleShape.m_radius = shapeDefinition.radius * meterPerPixel;
		circleShape.m_p = b2Vec2(
			shapeDefinition.center.x * meterPerPixel,
			shapeDefinition.center.y * meterPerPixel);

		return circleShape;
	}
}