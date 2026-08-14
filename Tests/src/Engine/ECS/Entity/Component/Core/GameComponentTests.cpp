#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


TEST_SUITE_BEGIN("GameComponent");


TEST_CASE("A newly added component is active by default")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto component = owner->AddComponent<DummyGameComponent>();

	CHECK(component->IsActive());
}

TEST_CASE("SetActive(false) makes IsActive report false")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto component = owner->AddComponent<DummyGameComponent>();
	component->SetActive(false);

	CHECK_FALSE(component->IsActive());
}

TEST_CASE("A component is inactive when its owning GameObject is inactive, even if its own flag is active")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto component = owner->AddComponent<DummyGameComponent>();
	owner->SetActive(false);

	CHECK_FALSE(component->IsActive());
}

TEST_CASE("Re-activating the owning GameObject restores an active component's IsActive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto component = owner->AddComponent<DummyGameComponent>();
	owner->SetActive(false);
	owner->SetActive(true);

	CHECK(component->IsActive());
}

TEST_CASE("MarkDirty is observable by a derived component")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto component = owner->AddComponent<DummyGameComponent>();

	CHECK_FALSE(component->IsDirty());

	component->MarkDirty();

	CHECK(component->IsDirty());
}


TEST_SUITE_END();