#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Engine;


TEST_SUITE_BEGIN("GameObject");


TEST_CASE("A freshly created GameObject already has a Transform")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	CHECK(obj->GetTransform() != nullptr);
}

TEST_CASE("A freshly created GameObject is active and has no parent or children")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	CHECK(obj->IsActive());
	CHECK(obj->GetParent() == nullptr);
	CHECK(obj->GetChildren().empty());
}

TEST_CASE("SetActive(false) makes IsActive report false")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	obj->SetActive(false);

	CHECK_FALSE(obj->IsActive());
}

TEST_CASE("SetParent attaches the child to the new parent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);

	CHECK(child->GetParent() == ObjectHandle<GameObject>::From(parent));
	REQUIRE(parent->GetChildren().size() == 1);
	CHECK(parent->GetChildren()[0] == ObjectHandle<GameObject>::From(child));
}

TEST_CASE("SetParent detaches the child from its previous parent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto oldParent = scene->Create<TestGameObject>();
	auto newParent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(oldParent);
	child->SetParent(newParent);

	CHECK(oldParent->GetChildren().empty());
	REQUIRE(newParent->GetChildren().size() == 1);
	CHECK(child->GetParent() == ObjectHandle<GameObject>::From(newParent));
}

TEST_CASE("IsChildOf is true for the direct parent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);

	CHECK(child->IsChildOf(parent));
}

TEST_CASE("IsChildOf(recursive=false) is false for a grandparent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto grandparent = scene->Create<TestGameObject>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	parent->SetParent(grandparent);
	child->SetParent(parent);

	CHECK_FALSE(child->IsChildOf(grandparent, false));
}

TEST_CASE("IsChildOf(recursive=true) is true for a grandparent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto grandparent = scene->Create<TestGameObject>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	parent->SetParent(grandparent);
	child->SetParent(parent);

	CHECK(child->IsChildOf(grandparent, true));
}

TEST_CASE("IsChildOf is false for unrelated objects")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto a = scene->Create<TestGameObject>();
	auto b = scene->Create<TestGameObject>();

	CHECK_FALSE(a->IsChildOf(b, true));
}

TEST_CASE("Deactivating a parent makes an active child report inactive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);
	parent->SetActive(false);

	CHECK_FALSE(child->IsActive());
}

TEST_CASE("Reactivating a parent restores an active child's IsActive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);

	parent->SetActive(false);

	CHECK(!child->IsActive());

	parent->SetActive(true);

	CHECK(child->IsActive());
}

TEST_CASE("Destroy detaches the object from its parent's children")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);
	child->Destroy();

	CHECK(parent->GetChildren().empty());
}

TEST_CASE("AddComponent links the new component to its owner")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	auto component = obj->AddComponent<DummyGameComponent>();

	CHECK(component->GetGameObject() == ObjectHandle<GameObject>::From(obj));
}

TEST_CASE("GetComponent finds a component added on the same object")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	auto added = obj->AddComponent<DummyGameComponent>();

	CHECK(obj->GetComponent<DummyGameComponent>() == added);
}

TEST_CASE("RemoveComponent makes GetComponent stop finding it")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto obj = scene->Create<TestGameObject>();

	auto added = obj->AddComponent<DummyGameComponent>();
	obj->RemoveComponent(added);

	CHECK(obj->GetComponent<DummyGameComponent>() == nullptr);
}

TEST_CASE("GetComponentInChildren finds a component on a direct child")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);
	auto added = child->AddComponent<DummyGameComponent>();

	CHECK(parent->GetComponentInChildren<DummyGameComponent>() == added);
}

TEST_CASE("GetComponentInChildren does not search grandchildren unless recursive")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto grandparent = scene->Create<TestGameObject>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	parent->SetParent(grandparent);
	child->SetParent(parent);
	child->AddComponent<DummyGameComponent>();

	CHECK(grandparent->GetComponentInChildren<DummyGameComponent>(false) == nullptr);
	CHECK(grandparent->GetComponentInChildren<DummyGameComponent>(true) != nullptr);
}

TEST_CASE("GetComponentInParent(includeSelf=false) finds a component on the parent")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);
	auto added = parent->AddComponent<DummyGameComponent>();

	CHECK(child->GetComponentInParent<DummyGameComponent>(false, false) == added);
}

TEST_CASE("GetComponentInParent(includeSelf=true) prefers the object's own component")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto parent = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();

	child->SetParent(parent);
	parent->AddComponent<DummyGameComponent>();
	auto ownComponent = child->AddComponent<DummyGameComponent>();

	CHECK(child->GetComponentInParent<DummyGameComponent>(true, true) == ownComponent);
}


TEST_SUITE_END();