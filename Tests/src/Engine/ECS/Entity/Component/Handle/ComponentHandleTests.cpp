#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


TEST_SUITE_BEGIN("ComponentHandle");


TEST_CASE("A default-constructed handle is invalid")
{
	ComponentHandle<DummyGameComponent> handle;

	CHECK(handle == nullptr);
	CHECK_FALSE(static_cast<bool>(handle));
}

TEST_CASE("operator-> and operator* dereference to the same instance")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(handle.operator->() == &(*handle));
}

TEST_CASE("operator() returns the same pointer as operator->")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(handle() == handle.operator->());
}

TEST_CASE("Two handles to the same slot compare equal")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	ComponentHandle<DummyGameComponent> copy = handle;

	CHECK(handle == copy);
	CHECK_FALSE(handle != copy);
}

TEST_CASE("Handles to different slots have different indices")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto first = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});
	auto second = bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(first.GetIndex() != second.GetIndex());
}

TEST_CASE("From<T> upcasts a derived handle to a base-typed handle")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto derived = bucket->AddComponent<DerivedDummyGameComponent>(ObjectHandle<GameObject>{});

	auto asBase = ComponentHandle<DummyGameComponent>::From(derived);

	CHECK(asBase != nullptr);
	CHECK(asBase.GetIndex() == derived.GetIndex());
}

TEST_CASE("The implicit upcast conversion operator produces an equivalent base handle")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto derived = bucket->AddComponent<DerivedDummyGameComponent>(ObjectHandle<GameObject>{});

	ComponentHandle<DummyGameComponent> asBase = derived;

	CHECK(asBase != nullptr);
	CHECK(asBase.GetIndex() == derived.GetIndex());
}

TEST_CASE("A handle constructed from a ComponentHandleBase of the wrong type is invalid")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto other = bucket->AddComponent<OtherDummyGameComponent>(ObjectHandle<GameObject>{});

	ComponentHandleBase base = other;
	auto wrongType = ComponentHandle<DummyGameComponent>::From(base);

	CHECK(wrongType == nullptr);
}

TEST_CASE("GetBucket returns the owning bucket for a valid handle and null for a default one")
{
	auto bucket = std::make_shared<ComponentBucket>();
	auto handle = bucket->AddComponent<DummyGameComponent>(ObjectHandle<GameObject>{});

	CHECK(handle.GetBucket() == bucket.get());

	ComponentHandle<DummyGameComponent> empty;

	CHECK(empty.GetBucket() == nullptr);
}


TEST_SUITE_END();