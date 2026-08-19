#include "Engine/ECS/Entity/Component/Handle/ComponentHandleBase.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Utilities/Delegates/Listeners/HandleListener.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;
using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("HandleListener");


TEST_CASE("Object-handle variant invokes while the object is alive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	auto called = 0;
	auto listener = HandleListener<>(obj, [&]() { ++called; });

	CHECK_FALSE(listener.IsExpired());

	listener.Invoke();

	CHECK(called == 1);
}


TEST_CASE("Object-handle variant expires and skips invocation once the object is destroyed")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	auto called = 0;
	auto listener = HandleListener<>(obj, [&]() { ++called; });

	scene->Destroy(obj.GetIndex());

	CHECK(listener.IsExpired());

	listener.Invoke();

	CHECK(called == 0);
}


TEST_CASE("Component-handle variant invokes while the component is alive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	auto called = 0;
	auto listener = HandleListener<>(comp, [&]() { ++called; });

	CHECK_FALSE(listener.IsExpired());

	listener.Invoke();

	CHECK(called == 1);
}


TEST_CASE("Component-handle variant expires and skips invocation once the component is removed")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	auto called = 0;
	auto listener = HandleListener<>(comp, [&]() { ++called; });

	obj->RemoveComponent(comp);

	CHECK(listener.IsExpired());

	listener.Invoke();

	CHECK(called == 0);
}


TEST_CASE("Default-constructed (null) object handle is expired from the start")
{
	auto listener = HandleListener<>(ObjectHandleBase{}, []() {});

	CHECK(listener.IsExpired());
}


TEST_CASE("MatchesIdentity matches the same object handle and rejects a different one")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto objA = scene->Create<TestGameObject>();
	auto objB = scene->Create<TestGameObject>();

	auto listener = HandleListener<>(objA, []() {});

	CHECK(listener.Matches<ObjectHandleBase>(objA));
	CHECK_FALSE(listener.Matches<ObjectHandleBase>(objB));
}


TEST_CASE("MatchesIdentity rejects a mismatched handle kind")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	auto objListener = HandleListener<>(obj, []() {});
	auto compListener = HandleListener<>(comp, []() {});

	// An object-handle listener's identity is never matched by a component handle, and vice versa.
	CHECK_FALSE(objListener.Matches<ComponentHandleBase>(comp));
	CHECK_FALSE(compListener.Matches<ObjectHandleBase>(obj));
}


TEST_SUITE_END();