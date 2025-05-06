#pragma once
#include "Math/Vector2.h"
#include <memory>


class b2World;
class b2ContactListener;


class PhysicsEngine2D
{
	friend class SubSystems;


private:
	static std::unique_ptr<b2World> world;


	int velocityIterations;

	int positionIterations;


	PhysicsEngine2D(const Vector2F& gravity);

	~PhysicsEngine2D() = default;

	PhysicsEngine2D(const PhysicsEngine2D&) = delete;

	PhysicsEngine2D(PhysicsEngine2D&&) = delete;


	void BeginFrame();


public:
	static void SetContactListener(b2ContactListener* listener);

	static Vector2F GetGravity();

	static void SetGravity(const Vector2F& newGravity);
};