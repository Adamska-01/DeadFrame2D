#include "Components/Collisions/Collider2D.h"
#include "Data/Collision/CollisionInfo.h"
#include "Data/Collision/ContactListener.h"


std::pair<Collider2D*, Collider2D*> ContactListener::GetCollidersFromContact(b2Contact* contact)
{
	auto* fixtureA = contact->GetFixtureA();
	auto* fixtureB = contact->GetFixtureB();

	auto* colA = reinterpret_cast<Collider2D*>(fixtureA->GetUserData().pointer);
	auto* colB = reinterpret_cast<Collider2D*>(fixtureB->GetUserData().pointer);

	if (!colA || !colB)
		return { nullptr, nullptr };

	return { colA, colB };
}

void ContactListener::BeginContact(b2Contact* contact)
{
	auto [colA, colB] = GetCollidersFromContact(contact);

	if (!colA || !colB)
		return;

	// Get world manifold for contact points and normal
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	auto contactPoint = Vector2F(worldManifold.points[0].x, worldManifold.points[0].y);
	auto normal = Vector2F(worldManifold.normal.x, worldManifold.normal.y);

	auto infoA = CollisionInfo
	{
		.contactPoint = contactPoint,
		.normal = normal,
		.thisCollider = colA,
		.otherCollider = colB,
	};
	
	auto infoB = CollisionInfo
	{
		.contactPoint = contactPoint,
		.normal = normal * -1,
		.thisCollider = colB,
		.otherCollider = colA
	};

	colA->OnCollisionEnterCallback(infoA);
	colB->OnCollisionEnterCallback(infoB);
}

void ContactListener::EndContact(b2Contact* contact)
{
	auto [colA, colB] = GetCollidersFromContact(contact);

	if (!colA || !colB)
		return;

	CollisionInfo infoA
	{
		.contactPoint = Vector2F(),
		.normal = Vector2F(),
		.thisCollider = colA,
		.otherCollider = colB
	};

	CollisionInfo infoB
	{
		.contactPoint = Vector2F(),
		.normal = Vector2F(),
		.thisCollider = colB,
		.otherCollider = colA
	};

	colA->OnCollisionExitCallback(infoA);
	colB->OnCollisionExitCallback(infoB);
}