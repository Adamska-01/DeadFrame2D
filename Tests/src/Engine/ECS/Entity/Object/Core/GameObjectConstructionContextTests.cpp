#include <doctest.h>
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <memory>
#include <stdexcept>


using namespace DF2D::Engine;


// Spawn a child and parent it to `this` directly from the constructor, relying on thisGameObject already being valid.
class SelfParentingTestGameObject : public GameObject
{
public:
	ObjectHandle<TestGameObject> spawnedChild;

	SelfParentingTestGameObject(FakeSceneHandleProvider& scene)
	{
		spawnedChild = scene.Create<TestGameObject>();
		spawnedChild->SetParent(thisGameObject);
	}
};


class AddsComponentInCtorTestGameObject : public GameObject
{
public:
	ComponentHandle<DummyGameComponent> addedComponent;

	AddsComponentInCtorTestGameObject()
	{
		addedComponent = AddComponent<DummyGameComponent>();
	}
};


class ThrowingTestGameObject : public GameObject
{
public:
	ThrowingTestGameObject()
	{
		throw std::runtime_error("boom");
	}
};


class ThrowsAfterAddingComponentTestGameObject : public GameObject
{
public:
	ThrowsAfterAddingComponentTestGameObject()
	{
		AddComponent<DummyGameComponent>();

		throw std::runtime_error("boom after component add");
	}
};


TEST_SUITE_BEGIN("GameObjectConstructionContext");


TEST_CASE("AddComponent called from a derived constructor links to a valid owner")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<AddsComponentInCtorTestGameObject>();

	REQUIRE(obj->addedComponent != nullptr);
	CHECK(obj->addedComponent->GetGameObject() == ObjectHandle<GameObject>::From(obj));
}

TEST_CASE("A GameObject can spawn and parent a child to itself from within its own constructor")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<SelfParentingTestGameObject>(*scene);

	REQUIRE(parent->spawnedChild != nullptr);
	CHECK(parent->spawnedChild->GetParent() == ObjectHandle<GameObject>::From(parent));
	REQUIRE(parent->GetChildren().size() == 1);
	CHECK(parent->GetChildren()[0] == ObjectHandle<GameObject>::From(parent->spawnedChild));
}

TEST_CASE("An exception thrown from a constructor propagates without corrupting the scene")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();

	CHECK_THROWS_AS(scene->Create<ThrowingTestGameObject>(), std::runtime_error);

	// The scene must still be usable afterward.
	auto obj = scene->Create<TestGameObject>();

	CHECK(obj != nullptr);
}

TEST_CASE("An exception thrown after AddComponent in the constructor still propagates cleanly")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();

	CHECK_THROWS_AS(scene->Create<ThrowsAfterAddingComponentTestGameObject>(), std::runtime_error);

	auto obj = scene->Create<TestGameObject>();

	CHECK(obj != nullptr);
}


TEST_SUITE_END();