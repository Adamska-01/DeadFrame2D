#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


TEST_SUITE_BEGIN("ComponentBucket");


TEST_CASE("AddComponent returns a valid handle resolving to the constructed instance")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	REQUIRE(handle != nullptr);
	CHECK(handle.operator->() != nullptr);
}

TEST_CASE("AddComponent with a null owner leaves the component unlinked")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(handle->GetGameObject() == nullptr);
}

TEST_CASE("GetComponent finds a component by type")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto added = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	auto found = bucket->GetComponent<DummyGameComponent>();

	CHECK(found == added);
}

TEST_CASE("GetComponent ignores components of a different type")
{
	auto bucket = std::make_shared<ComponentBucket>();

	bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});
	auto found = bucket->GetComponent<DummyGameComponent>();

	CHECK(found == nullptr);
}

TEST_CASE("GetComponent matches a derived type against a base-type request")
{
	auto bucket = std::make_shared<ComponentBucket>();

	bucket->AddComponent<DerivedDummyGameComponent>(ObjectHandle<GameObject>{});
	auto found = bucket->GetComponent<DummyGameComponent>();

	CHECK(found != nullptr);
}

TEST_CASE("GetComponent on an empty bucket returns an invalid handle")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto found = bucket->GetComponent<DummyGameComponent>();

	CHECK(found == nullptr);
}

TEST_CASE("RemoveComponent invalidates the handle")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	bucket->RemoveComponent(handle);

	CHECK(handle == nullptr);
}

TEST_CASE("RemoveComponent makes GetComponent stop finding the removed instance")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	bucket->RemoveComponent(handle);

	CHECK(bucket->GetComponent<DummyGameComponent>() == nullptr);
}

TEST_CASE("A freed slot is reused by the next AddComponent, bumping the generation")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto first = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	auto firstIndex = first.GetIndex();
	auto firstGeneration = first.GetGeneration();

	bucket->RemoveComponent(first);

	auto second = bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(second.GetIndex() == firstIndex);
	CHECK(second.GetGeneration() == firstGeneration + 1);
}

TEST_CASE("The stale handle to a reused slot stays invalid after the slot is reused")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto first = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	bucket->RemoveComponent(first);
	bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(first == nullptr);
}

TEST_CASE("Multiple components of distinct types coexist in the same bucket")
{
	auto bucket = std::make_shared<ComponentBucket>();

	auto dummy = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	auto other = bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(dummy != nullptr);
	CHECK(other != nullptr);
	CHECK(dummy.GetIndex() != other.GetIndex());
}

TEST_CASE("Destroying the bucket invalidates every handle it produced")
{
	ComponentHandle<DummyGameComponent> handle;

	{
		auto bucket = std::make_shared<ComponentBucket>();
		handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

		REQUIRE(handle != nullptr);
	}

	CHECK(handle == nullptr);
}


TEST_SUITE_END();