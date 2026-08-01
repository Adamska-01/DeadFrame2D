#pragma once
#include "Core/Math/Vector2.h"
#include "Data/Systems/Physics/FixtureID.h"
#include "DF2D_API.h"


namespace DF2D::Core
{
	/**
	 * @brief Receives contact events from a physics backend, expressed in engine types only.
	 *
	 * Contact points are in pixels; normals are unit vectors pointing from fixture A to fixture B.
	 */
	class DF2D_API IPhysicsContactSink
	{
	public:
		virtual ~IPhysicsContactSink() = default;


		virtual void OnContactBegin(Data::FixtureID fixtureA, Data::FixtureID fixtureB, const Vector2F& contactPoint, const Vector2F& normal) = 0;

		virtual void OnContactEnd(Data::FixtureID fixtureA, Data::FixtureID fixtureB) = 0;
	};
}