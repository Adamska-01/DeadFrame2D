#pragma once
#include "Data/Collision/PhysicsMaterial.h"
#include "Data/Physics/BodyDefinition2D.h"
#include "Data/Physics/BodyType2D.h"
#include <box2d/b2_body.h>
#include <cassert>


namespace PhysicsConversion
{
	/**
	 * @brief Converts a custom BodyType2D to Box2D's b2BodyType.
	 * @param type The custom BodyType2D value.
	 * @return Corresponding b2BodyType.
	 */
	inline b2BodyType ToB2BodyType(BodyType2D type)
	{
		switch (type)
		{
		case BodyType2D::Static:
			return b2_staticBody;

		case BodyType2D::Kinematic:
			return b2_kinematicBody;

		case BodyType2D::Dynamic:
			return b2_dynamicBody;

		default:
			return b2_staticBody;
		}
	}

	/**
	 * @brief Converts BodyDefinition2D wrapper into a b2BodyDef instance for use with Box2D.
	 */
	inline b2BodyDef ToB2BodyDef(const BodyDefinition2D& bodyDef)
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
	inline b2FixtureDef ToB2FixtureDef(const PhysicsMaterial& physicsMaterial, uintptr_t userDataPtr)
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
}