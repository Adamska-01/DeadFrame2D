#include "Components/Audio/AudioListener.h"
#include "Components/Audio/AudioSource.h"
#include "Components/Collisions/Collider2D.h"
#include "Data/Collision/CollisionInfo.h"
#include "Data/Collision/ContactListener.h"
#include "GameObject.h"


std::pair<ContactEventProvider*, ContactEventProvider*> ContactListener::GetUserDataFromContact(b2Contact* contact)
{
	auto* fixtureA = contact->GetFixtureA();
	auto* fixtureB = contact->GetFixtureB();

	auto* objA = reinterpret_cast<ContactEventProvider*>(fixtureA->GetUserData().pointer);
	auto* objB = reinterpret_cast<ContactEventProvider*>(fixtureB->GetUserData().pointer);

	return { objA, objB };
}

void ContactListener::BeginContact(b2Contact* contact)
{
	auto [colA, colB] = GetUserDataFromContact(contact);

	if (colA == nullptr || colB == nullptr)
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
		.thisGameObject = colA->GetGameObject(),
		.otherGameObject = colB->GetGameObject(),
	};

	auto infoB = CollisionInfo
	{
		.contactPoint = contactPoint,
		.normal = normal * -1,
		.thisGameObject = colB->GetGameObject(),
		.otherGameObject = colA->GetGameObject()
	};

	colA->InvokeCollisionEnter(infoA);
	colB->InvokeCollisionEnter(infoB);
}

void ContactListener::EndContact(b2Contact* contact)
{
	auto [colA, colB] = GetUserDataFromContact(contact);

	if (!colA || !colB)
		return;

	CollisionInfo infoA
	{
		.contactPoint = Vector2F(),
		.normal = Vector2F(),
		.thisGameObject = colA->GetGameObject(),
		.otherGameObject = colB->GetGameObject(),
	};

	CollisionInfo infoB
	{
		.contactPoint = Vector2F(),
		.normal = Vector2F(),
		.thisGameObject = colB->GetGameObject(),
		.otherGameObject = colA->GetGameObject()
	};

	colA->InvokeCollisionExit(infoA);
	colB->InvokeCollisionExit(infoB);
}