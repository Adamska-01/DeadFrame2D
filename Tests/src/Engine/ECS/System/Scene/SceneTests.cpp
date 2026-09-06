#include "Engine/ECS/Entity/Component/Core/GameComponent.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Engine;


namespace
{
	// Constructs successfully as far as GameObject's own base ctor runs (adds Transform),
	// then throws from its own ctor body - exercises Scene::Instantiate's cleanup path.
	class ThrowingGameObject : public GameObject
	{
	public:
		ThrowingGameObject()
		{
			throw std::runtime_error("boom");
		}
	};

	class MarkerComponent : public GameComponent
	{
		TYPE_INFO(MarkerComponent, GameComponent);

	public:
		int initCallCount = 0;

		void Init() override
		{
			initCallCount++;
		}
	};

	// Spawns a child (with a MarkerComponent) from inside its own Init() - exercises
	// Scene::Init()'s re-batching of objects spawned while other objects are still
	// draining their own Init/Start pass.
	class SpawningOnInitComponent : public GameComponent
	{
		TYPE_INFO(SpawningOnInitComponent, GameComponent);

	public:
		TestScene* scene = nullptr;

		ComponentHandle<MarkerComponent> spawnedMarker;

		void Init() override
		{
			auto child = scene->Spawn<TestGameObject>();

			spawnedMarker = child->AddComponent<MarkerComponent>();
		}
	};

	// Spawns a sibling from inside its own Update() - exercises the fix for entries
	// reallocating (vector growth) while Scene::Update is mid-iteration.
	class SpawningOnUpdateComponent : public GameComponent
	{
		TYPE_INFO(SpawningOnUpdateComponent, GameComponent);

	public:
		TestScene* scene = nullptr;

		bool hasSpawned = false;

		int updateCallCount = 0;

		void Update(float deltaTime) override
		{
			updateCallCount++;

			if (hasSpawned)
				return;

			hasSpawned = true;

			scene->Spawn<TestGameObject>();
		}
	};
}


TEST_SUITE_BEGIN("Scene");


TEST_CASE("Spawn returns a valid handle for a freshly constructed object")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();

	CHECK(obj != nullptr);
	CHECK(obj->GetTransform() != nullptr);
}

TEST_CASE("A freed slot is reused with a bumped generation")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto first = scene->Spawn<TestGameObject>();
	auto firstIndex = first.GetIndex();
	auto firstGeneration = first.GetGeneration();

	first->Destroy();
	scene->Update(0.0f);

	auto second = scene->Spawn<TestGameObject>();

	CHECK(second.GetIndex() == firstIndex);
	CHECK(second.GetGeneration() == firstGeneration + 1);
	CHECK(first == nullptr);
}

TEST_CASE("Destroy invalidates the handle immediately, before Update processes it")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();

	obj->Destroy();

	CHECK(obj == nullptr);
}

TEST_CASE("Instantiate cleans up and rethrows when the constructor throws")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	CHECK_THROWS_AS(scene->Spawn<ThrowingGameObject>(), std::runtime_error);

	// The failed slot must be recycled, not leaked.
	auto obj = scene->Spawn<TestGameObject>();

	REQUIRE(obj != nullptr);
	CHECK(obj.GetIndex() == 0);
}

TEST_CASE("SetParent removes the child from the scene's root set")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto parent = scene->Spawn<TestGameObject>();
	auto child = scene->Spawn<TestGameObject>();

	child->AddComponent<DummyGameComponent>();
	child->SetParent(parent);

	// If the child were still (incorrectly) tracked as a root as well as a child of
	// parent, it would be reachable twice from Scene's traversal.
	auto results = scene->FindObjectsOfType<DummyGameComponent>();

	CHECK(results.size() == 1);
}

TEST_CASE("FindObjectOfType finds a component on a spawned object")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();
	auto added = obj->AddComponent<DummyGameComponent>();

	auto found = scene->FindObjectOfType<DummyGameComponent>();

	CHECK(found != nullptr);
	CHECK(found == added);
}

TEST_CASE("FindObjectOfType returns an invalid handle when nothing matches")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	CHECK(scene->FindObjectOfType<DummyGameComponent>() == nullptr);
}

TEST_CASE("Scene::Init calls Init/Start on components of objects spawned before it ran")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	scene->Init();

	CHECK(comp->initCallCount == 1);
}

TEST_CASE("Scene::Init drains objects spawned by another object's own Init")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();
	auto spawner = obj->AddComponent<SpawningOnInitComponent>();
	spawner->scene = scene.get();

	scene->Init();

	REQUIRE(spawner->spawnedMarker != nullptr);
	CHECK(spawner->spawnedMarker->initCallCount == 1);
}

TEST_CASE("Scene::Update calls Update on active components")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto obj = scene->Spawn<TestGameObject>();
	auto comp = obj->AddComponent<DummyGameComponent>();

	scene->Init();
	scene->Update(0.5f);

	CHECK(comp->updateCallCount == 1);
	CHECK(comp->lastDeltaTime == doctest::Approx(0.5f));
}

TEST_CASE("Update survives entries reallocating from a component that spawns during Update")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	// Churn entries so it isn't sitting at a trivially small capacity.
	for (int i = 0; i < 8; ++i)
		scene->Spawn<TestGameObject>();

	auto spawner = scene->Spawn<TestGameObject>();
	auto spawningComp = spawner->AddComponent<SpawningOnUpdateComponent>();
	spawningComp->scene = scene.get();

	scene->Init();

	CHECK_NOTHROW(scene->Update(0.016f));
	CHECK_NOTHROW(scene->Update(0.016f));

	CHECK(spawningComp->updateCallCount == 2);
}


TEST_CASE("A child that survives its parent's destruction is still reachable by the scene")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto parent = scene->Spawn<TestGameObject>();

	auto first = scene->Spawn<TestGameObject>();
	auto second = scene->Spawn<TestGameObject>();
	auto third = scene->Spawn<TestGameObject>();

	first->SetParent(parent);
	second->SetParent(parent);
	third->SetParent(parent);

	scene->Init();

	parent->Destroy();

	// A surviving child is an orphan: its parent handle has gone stale, so it now reports no parent
	// and looks like a root, but it was never added to the scene's root list. Nothing would ever
	// update, draw or destroy it again.
	scene->Update(0.016f);

	auto orphaned = 0;

	for (const auto& child : { first, second, third })
	{
		if (child != nullptr && child->GetParent() == nullptr)
		{
			orphaned++;
		}
	}

	CHECK(orphaned == 0);
}


TEST_CASE("Detaching an object promotes it back to a root of the scene")
{
	EventDispatcher dispatcher;
	auto scene = std::make_shared<TestScene>(&dispatcher);

	auto parent = scene->Spawn<TestGameObject>();
	auto child = scene->Spawn<TestGameObject>();

	child->SetParent(parent);

	REQUIRE(child->GetParent() != nullptr);

	// Until now this crashed on a null dereference, which left the scene's "child became a root"
	// path unreachable. Detaching has to put the object back in the root list or it stops being
	// updated and drawn.
	child->SetParent(ObjectHandle<GameObject>());

	CHECK(child->GetParent() == nullptr);
	CHECK(parent->GetChildren().empty());

	// Destroying the former parent must now leave the detached object completely alone.
	parent->Destroy();

	CHECK(child != nullptr);
}


TEST_SUITE_END();