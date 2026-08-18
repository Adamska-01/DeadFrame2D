#include "Data/Components/UI/Layout/LayoutPadding.h"
#include "Engine/ECS/Component/UI/Layout/Abstractions/LayoutGroup.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	class TestLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(TestLayoutGroup, LayoutGroup);


	public:
		int updateLayoutCallCount = 0;


		TestLayoutGroup()
			: LayoutGroup(5.0f, LayoutPadding{})
		{
		}

		void UpdateLayout() override
		{
			updateLayoutCallCount++;
		}

		bool IsDirty() const
		{
			return isDirty;
		}
	};
}


TEST_SUITE_BEGIN("LayoutGroup");


TEST_CASE("Start invokes UpdateLayout once")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto layout = owner->AddComponent<TestLayoutGroup>();
	layout->Start();

	CHECK(layout->updateLayoutCallCount == 1);
}

TEST_CASE("Update does not call UpdateLayout while not dirty")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto layout = owner->AddComponent<TestLayoutGroup>();
	layout->Start();

	layout->Update(0.016f);
	layout->Update(0.016f);

	CHECK(layout->updateLayoutCallCount == 1);
}

TEST_CASE("Adding a child game object marks the layout dirty and Update consumes it")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto layout = owner->AddComponent<TestLayoutGroup>();
	layout->Start();

	CHECK(!layout->IsDirty());

	auto child = scene->Create<TestGameObject>();
	child->SetParent(owner);

	CHECK(layout->IsDirty());

	layout->Update(0.016f);

	CHECK(layout->updateLayoutCallCount == 2);
	CHECK_FALSE(layout->IsDirty());
}

TEST_CASE("A child's active state changing marks the layout dirty")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();
	child->SetParent(owner);

	auto layout = owner->AddComponent<TestLayoutGroup>();
	layout->Start();

	CHECK(!layout->IsDirty());

	child->SetActive(false);

	CHECK(layout->IsDirty());

	layout->Update(0.016f);

	CHECK(layout->updateLayoutCallCount == 2);
}

TEST_CASE("Destroying a direct child marks the layout dirty")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto child = scene->Create<TestGameObject>();
	child->SetParent(owner);

	auto layout = owner->AddComponent<TestLayoutGroup>();
	layout->Start();

	CHECK(!layout->IsDirty());

	child->Destroy();

	CHECK(layout->IsDirty());

	layout->Update(0.016f);

	CHECK(layout->updateLayoutCallCount == 2);
}


TEST_SUITE_END();