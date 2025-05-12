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

	auto infoA = CollisionInfo
	{
		.contactPoint = Vector2F(worldManifold.points[0].x, worldManifold.points[0].y),
		.normal = Vector2F(worldManifold.normal.x, worldManifold.normal.y),
		.otherCollider = colA,
		.otherGameObject = colA->GetGameObject(),
	};
	
	auto infoB = CollisionInfo
	{
		.contactPoint = Vector2F(worldManifold.points[0].x, worldManifold.points[0].y),
		.normal = Vector2F(-worldManifold.normal.x, -worldManifold.normal.y),
		.otherCollider = colB,
		.otherGameObject = colB->GetGameObject(),
	};

	colA->OnCollisionEnterCallback(infoA);
	colB->OnCollisionEnterCallback(infoB);
}

void ContactListener::EndContact(b2Contact* contact)
{
	auto [colA, colB] = GetCollidersFromContact(contact);

	if (!colA || !colB)
		return;

	// Even though the contact has ended, we can still build a basic CollisionInfo.
	// Note: contact point and normal might not be meaningful here, but can be set to defaults.
	CollisionInfo infoA
	{
		.contactPoint = Vector2F(), // optionally, keep it zero since contact has ended
		.normal = Vector2F(),       // no direction after separation
		.otherCollider = colB,
		.otherGameObject = colB->GetGameObject(),
	};

	CollisionInfo infoB
	{
		.contactPoint = Vector2F(),
		.normal = Vector2F(),
		.otherCollider = colA,
		.otherGameObject = colA->GetGameObject(),
	};

	colA->OnCollisionExitCallback(infoA);
	colB->OnCollisionExitCallback(infoB);
}