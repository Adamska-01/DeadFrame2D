#include "Data/Components/UI/Layout/LayoutPadding.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Layout/VerticalLayoutGroup.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


TEST_SUITE_BEGIN("VerticalLayoutGroup");


TEST_CASE("Stacks two children top-to-bottom, centered around the owner")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<VerticalLayoutGroup>(10.0f, LayoutPadding{});

	auto childA = scene->Create<TestGameObject>();
	childA->SetParent(owner);
	auto uiA = childA->AddComponent<UIComponent>();
	uiA->SetWidgetSize(Vector2F(30.0f, 20.0f));

	auto childB = scene->Create<TestGameObject>();
	childB->SetParent(owner);
	auto uiB = childB->AddComponent<UIComponent>();
	uiB->SetWidgetSize(Vector2F(30.0f, 40.0f));

	layout->UpdateLayout();

	// totalHeight = 20 + 40 + spacing(10) = 70; start y = -35
	CHECK(childA->GetTransform()->GetLocalPosition().y == doctest::Approx(-25.0f));
	CHECK(childB->GetTransform()->GetLocalPosition().y == doctest::Approx(15.0f));
}

TEST_CASE("Skips inactive children when computing the layout")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<VerticalLayoutGroup>(10.0f, LayoutPadding{});

	auto childA = scene->Create<TestGameObject>();
	childA->SetParent(owner);
	auto uiA = childA->AddComponent<UIComponent>();
	uiA->SetWidgetSize(Vector2F(30.0f, 20.0f));

	auto childB = scene->Create<TestGameObject>();
	childB->SetParent(owner);
	auto uiB = childB->AddComponent<UIComponent>();
	uiB->SetWidgetSize(Vector2F(30.0f, 40.0f));
	childB->SetActive(false);

	layout->UpdateLayout();

	// Only childA counts: totalHeight = 20, start y = -10, centered at -10 + 10 = 0
	CHECK(childA->GetTransform()->GetLocalPosition().y == doctest::Approx(0.0f));
}

TEST_CASE("Does nothing with no UIComponent children")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<VerticalLayoutGroup>(10.0f, LayoutPadding{});

	CHECK_NOTHROW(layout->UpdateLayout());
}


TEST_SUITE_END();