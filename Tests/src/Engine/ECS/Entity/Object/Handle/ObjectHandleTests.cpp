#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


// Exercises the upcast path (ObjectHandle<Derived> -> ObjectHandle<Base>).
class ChildTestGameObject : public TestGameObject
{
};


TEST_SUITE_BEGIN("ObjectHandle");


TEST_CASE("A default-constructed handle is invalid")
{
	ObjectHandle<TestGameObject> handle;

	CHECK(handle == nullptr);
	CHECK_FALSE(static_cast<bool>(handle));
}

TEST_CASE("A handle produced by the scene provider is valid and dereferenceable")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto handle = scene->Create<TestGameObject>();

	REQUIRE(handle != nullptr);
	CHECK(handle.operator->() != nullptr);
	CHECK(&(*handle) == handle.operator->());
}

TEST_CASE("Two handles to the same object compare equal")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto handle = scene->Create<TestGameObject>();

	ObjectHandle<TestGameObject> copy = handle;

	CHECK(handle == copy);
	CHECK_FALSE(handle != copy);
}

TEST_CASE("Handles to different objects have different indices")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto first = scene->Create<TestGameObject>();
	auto second = scene->Create<TestGameObject>();

	CHECK(first != second);
	CHECK(first.GetIndex() != second.GetIndex());
}

TEST_CASE("Destroying the object invalidates its handle")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto handle = scene->Create<TestGameObject>();

	scene->Destroy(handle.GetIndex());

	CHECK(handle == nullptr);
	CHECK(!handle); // Testing bool operators
}

TEST_CASE("Destroying and recreating a slot bumps the generation, invalidating the stale handle")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto first = scene->Create<TestGameObject>();
	auto firstIndex = first.GetIndex();

	scene->Destroy(firstIndex);

	CHECK(first == nullptr);
	CHECK(!first); // Testing bool operators
}

TEST_CASE("Destroying the scene provider invalidates every handle it produced")
{
	ObjectHandle<TestGameObject> handle;

	{
		auto scene = std::make_shared<FakeSceneHandleProvider>();
		handle = scene->Create<TestGameObject>();

		REQUIRE(handle != nullptr);
	}

	CHECK(handle == nullptr);
	CHECK(!handle); // Testing bool operators
}

TEST_CASE("From<T> upcasts a derived handle to a base-typed handle")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto derived = scene->Create<ChildTestGameObject>();

	auto asBase = ObjectHandle<TestGameObject>::From(derived);

	CHECK(asBase != nullptr);
	CHECK(asBase == derived);
	CHECK(asBase.GetIndex() == derived.GetIndex()); // "asBase == derived" also checks the index
}

TEST_CASE("The implicit upcast conversion operator produces an equivalent base handle")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto derived = scene->Create<ChildTestGameObject>();

	ObjectHandle<TestGameObject> asBase = derived;

	CHECK(asBase != nullptr);
	CHECK(asBase == derived);
	CHECK(asBase.GetIndex() == derived.GetIndex()); // "asBase == derived" also checks the index
}

TEST_CASE("GetSceneHandleProvider returns the owning provider for a valid handle and null for a default one")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto handle = scene->Create<TestGameObject>();

	CHECK(handle.GetSceneHandleProvider() == scene.get());

	ObjectHandle<TestGameObject> empty;

	CHECK(empty.GetSceneHandleProvider() == nullptr);
}


TEST_SUITE_END();