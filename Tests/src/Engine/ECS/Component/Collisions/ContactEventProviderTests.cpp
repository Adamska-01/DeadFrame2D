#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include "Engine/ECS/Component/Transform.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	class PublicContactEventProvider : public ContactEventProvider
	{
		TYPE_INFO(PublicContactEventProvider, ContactEventProvider);


	public:
		using ContactEventProvider::InvokeCollisionEnter;
		using ContactEventProvider::InvokeCollisionExit;
	};
}


TEST_SUITE_BEGIN("ContactEventProvider");


TEST_CASE("RegisterContactEnterHandler(ObjectHandle) invokes the handler with the broadcast info")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto provider = owner->AddComponent<PublicContactEventProvider>();

	auto listenerOwner = scene->Create<TestGameObject>();

	auto callCount = 0;
	auto received = CollisionInfo{};

	provider->RegisterContactEnterHandler(listenerOwner, [&](const CollisionInfo& info)
		{
			callCount++;
			received = info;
		});

	auto info = CollisionInfo{ .contactPoint = Vector2F(1.0f, 2.0f), .normal = Vector2F(0.0f, 1.0f) };

	provider->InvokeCollisionEnter(info);

	CHECK(callCount == 1);
	CHECK(received.contactPoint.x == doctest::Approx(1.0f));
	CHECK(received.contactPoint.y == doctest::Approx(2.0f));
}

TEST_CASE("DeregisterContactEnterHandler(ObjectHandle) stops further invocations")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto provider = owner->AddComponent<PublicContactEventProvider>();

	auto listenerOwner = scene->Create<TestGameObject>();

	auto callCount = 0;

	provider->RegisterContactEnterHandler(listenerOwner, [&](const CollisionInfo&) { callCount++; });
	provider->DeregisterContactEnterHandler(listenerOwner);

	provider->InvokeCollisionEnter(CollisionInfo{});

	CHECK(callCount == 0);
}

TEST_CASE("RegisterContactExitHandler(ComponentHandle) invokes the handler, DeregisterContactExitHandler stops it")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto provider = owner->AddComponent<PublicContactEventProvider>();

	auto listenerOwner = scene->Create<TestGameObject>();
	auto listenerIdentity = listenerOwner->GetTransform();

	auto callCount = 0;

	provider->RegisterContactExitHandler(listenerIdentity, [&](const CollisionInfo&) { callCount++; });

	provider->InvokeCollisionExit(CollisionInfo{});
	CHECK(callCount == 1);

	provider->DeregisterContactExitHandler(listenerIdentity);

	provider->InvokeCollisionExit(CollisionInfo{});
	CHECK(callCount == 1);
}

TEST_CASE("Multiple registered listeners are all broadcast to")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto provider = owner->AddComponent<PublicContactEventProvider>();

	auto listenerOwnerA = scene->Create<TestGameObject>();
	auto listenerOwnerB = scene->Create<TestGameObject>();

	auto callCountA = 0;
	auto callCountB = 0;

	provider->RegisterContactEnterHandler(listenerOwnerA, [&](const CollisionInfo&) { callCountA++; });
	provider->RegisterContactEnterHandler(listenerOwnerB->GetTransform(), [&](const CollisionInfo&) { callCountB++; });

	provider->InvokeCollisionEnter(CollisionInfo{});

	CHECK(callCountA == 1);
	CHECK(callCountB == 1);
}

TEST_CASE("A listener whose owning object was destroyed is skipped automatically")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto provider = owner->AddComponent<PublicContactEventProvider>();

	auto listenerOwner = scene->Create<TestGameObject>();

	auto callCount = 0;

	provider->RegisterContactEnterHandler(listenerOwner, [&](const CollisionInfo&) { callCount++; });

	scene->Destroy(listenerOwner.GetIndex());

	CHECK_NOTHROW(provider->InvokeCollisionEnter(CollisionInfo{}));

	CHECK(callCount == 0);
}


TEST_SUITE_END();