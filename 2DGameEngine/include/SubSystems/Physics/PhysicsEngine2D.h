#pragma once
#include "Math/Vector2.h"
#include "SubSystems/Abstractions/ISubSystem.h"
#include <memory>


struct b2BodyDef;
class b2Body;
class b2World;
class b2ContactListener;


class PhysicsEngine2D : public ISubSystem
{
	friend class SubSystems;


private:
	static std::unique_ptr<b2World> world;


	PhysicsEngine2D(const Vector2F& gravity);

	~PhysicsEngine2D();

	PhysicsEngine2D(const PhysicsEngine2D&) = delete;

	PhysicsEngine2D(PhysicsEngine2D&&) = delete;


	virtual void Update(float deltaTime) override;

	virtual void BeginFrame() override;

	virtual void EndUpdate() override;

	virtual void EndDraw() override;


public:
	static Vector2F GetGravity();

	static void SetGravity(const Vector2F& newGravity);

	static b2Body* CreateBody(const b2BodyDef* bodyDef);

	static void DestroyBody(b2Body* bodyToDestroy);
};