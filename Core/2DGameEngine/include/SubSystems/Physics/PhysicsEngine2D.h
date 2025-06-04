#pragma once
#include "Math/Vector2.h"
#include <memory>


struct b2BodyDef;
class b2Body;
class b2World;
class b2ContactListener;


class PhysicsEngine2D
{
	friend class SubSystems;


private:
	static std::unique_ptr<b2World> world;


	PhysicsEngine2D(const Vector2F& gravity);

	~PhysicsEngine2D();

	PhysicsEngine2D(const PhysicsEngine2D&) = delete;

	PhysicsEngine2D(PhysicsEngine2D&&) = delete;


	// TODO: Make this an interface and share it with other subsystems
	void BeginFrame();

	void EndFrame();


public:
	static Vector2F GetGravity();

	static void SetGravity(const Vector2F& newGravity);

	static b2Body* CreateBody(const b2BodyDef* bodyDef);

	static void DestroyBody(b2Body* bodyToDestroy);
};