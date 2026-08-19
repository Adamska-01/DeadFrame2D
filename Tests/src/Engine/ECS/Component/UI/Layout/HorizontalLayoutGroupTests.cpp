#include "Data/Components/UI/Layout/LayoutPadding.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Layout/HorizontalLayoutGroup.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


TEST_SUITE_BEGIN("HorizontalLayoutGroup");


TEST_CASE("Lines up two children left-to-right, centered around the owner")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<HorizontalLayoutGroup>(10.0f, LayoutPadding{});

	auto childA = scene->Create<TestGameObject>();
	childA->SetParent(owner);
	auto uiA = childA->AddComponent<UIComponent>();
	uiA->SetWidgetSize(Vector2F(20.0f, 30.0f));

	auto childB = scene->Create<TestGameObject>();
	childB->SetParent(owner);
	auto uiB = childB->AddComponent<UIComponent>();
	uiB->SetWidgetSize(Vector2F(40.0f, 30.0f));

	layout->UpdateLayout();

	// totalWidth = 20 + 40 + spacing(10) = 70; start x = -35 + firstHalfWidth(10) = -25
	CHECK(childA->GetTransform()->GetLocalPosition().x == doctest::Approx(-25.0f));
	// next x = -25 + 20 + 10 = 5
	CHECK(childB->GetTransform()->GetLocalPosition().x == doctest::Approx(5.0f));
}

TEST_CASE("Skips inactive children when computing the layout")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<HorizontalLayoutGroup>(10.0f, LayoutPadding{});

	auto childA = scene->Create<TestGameObject>();
	childA->SetParent(owner);
	auto uiA = childA->AddComponent<UIComponent>();
	uiA->SetWidgetSize(Vector2F(20.0f, 30.0f));
	childA->SetActive(false);

	auto childB = scene->Create<TestGameObject>();
	childB->SetParent(owner);
	auto uiB = childB->AddComponent<UIComponent>();
	uiB->SetWidgetSize(Vector2F(40.0f, 30.0f));

	layout->UpdateLayout();

	// Only childB counts: totalWidth = 40, start x = -20 + halfWidth(20) = 0
	CHECK(childB->GetTransform()->GetLocalPosition().x == doctest::Approx(0.0f));
}

TEST_CASE("Does nothing with no UIComponent children")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();
	auto layout = owner->AddComponent<HorizontalLayoutGroup>(10.0f, LayoutPadding{});

	CHECK_NOTHROW(layout->UpdateLayout());
}


TEST_SUITE_END();