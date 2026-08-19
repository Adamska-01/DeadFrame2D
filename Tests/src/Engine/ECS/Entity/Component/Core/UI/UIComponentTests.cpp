#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// Exposes the protected isDirty flag GameComponent already tracks, mirroring
	// DummyGameComponent::IsDirty() so dirty-marking can be asserted directly.
	class TestUIComponent : public UIComponent
	{
		TYPE_INFO(TestUIComponent, UIComponent);


	public:
		bool IsDirty() const
		{
			return isDirty;
		}
	};
}


TEST_SUITE_BEGIN("UIComponent");


TEST_CASE("Init marks the component dirty")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	CHECK(comp->IsDirty());
}

TEST_CASE("Update throws when there is no Canvas anywhere in the parent chain")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	CHECK_THROWS_AS(comp->Update(0.016f), std::invalid_argument);
}

TEST_CASE("Update does not throw once a Canvas ancestor is resolved")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto canvasOwner = scene->Create<TestGameObject>();
	canvasOwner->AddComponent<Canvas>();

	auto owner = scene->Create<TestGameObject>();
	owner->SetParent(canvasOwner);

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	CHECK_NOTHROW(comp->Update(0.016f));
}

TEST_CASE("Reparenting under a Canvas re-resolves parentCanvas so Update stops throwing")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto canvasOwner = scene->Create<TestGameObject>();
	canvasOwner->AddComponent<Canvas>();

	auto owner = scene->Create<TestGameObject>();

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	CHECK_THROWS_AS(comp->Update(0.016f), std::invalid_argument);

	owner->SetParent(canvasOwner);

	CHECK_NOTHROW(comp->Update(0.016f));
}

TEST_CASE("Default widget size is Vector2F::One")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	CHECK(comp->GetWidgetSize().x == doctest::Approx(1.0f));
	CHECK(comp->GetWidgetSize().y == doctest::Approx(1.0f));
}

TEST_CASE("GetWidgetSize scales widgetSize by the transform's world scale")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto comp = owner->AddComponent<TestUIComponent>();
	comp->Init();

	comp->SetWidgetSize(Vector2F(50.0f, 20.0f));
	owner->GetTransform()->SetLocalScale(Vector2F(2.0f, 3.0f));

	CHECK(comp->GetWidgetSize().x == doctest::Approx(100.0f));
	CHECK(comp->GetWidgetSize().y == doctest::Approx(60.0f));
}


TEST_SUITE_END();