#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Layout/GridLayoutGroup.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include "Mocks/Context/Systems/UI/MockUIBackend.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// A canvas needs a renderer for its resolution and a UI manager for its context; nothing else in
	// these tests touches the rest of the core context.
	struct LayoutGroupFixture
	{
		EventDispatcher dispatcher;

		std::unique_ptr<Renderer> renderer;

		MockTimeProvider time;

		MockUIBackend* mock = nullptr;

		std::unique_ptr<UIManager> uiManager;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> canvasObject;

		ComponentHandle<Canvas> canvas;

		ObjectHandle<TestGameObject> groupObject;

		ComponentHandle<GridLayoutGroup> group;


		LayoutGroupFixture()
		{
			renderer = std::make_unique<Renderer>(std::make_unique<MockRenderBackend>());

			auto ownedBackend = std::make_unique<MockUIBackend>();
			mock = ownedBackend.get();
			uiManager = std::make_unique<UIManager>(std::move(ownedBackend), &time);

			scene = std::make_shared<TestScene>(&dispatcher);
			scene->SetCoreContext(CoreContext
				{
					.renderer = renderer.get(),
					.uiManager = uiManager.get()
				});

			canvasObject = scene->Spawn<TestGameObject>();
			canvas = canvasObject->AddComponent<Canvas>();

			groupObject = scene->Spawn<TestGameObject>();
			groupObject->SetParent(canvasObject);
			group = groupObject->AddComponent<GridLayoutGroup>();
			group->SetSpacing(16.0f);
			group->SetPadding(LayoutPadding{ .left = 4.0f, .right = 4.0f, .top = 8.0f, .bottom = 8.0f });

			scene->Init();
		}
	};
}


TEST_SUITE_BEGIN("LayoutGroup");


TEST_CASE("Spacing is authored in dp, not px, so it scales with the canvas's UI scale factor")
{
	LayoutGroupFixture fixture;

	auto elementId = fixture.group->GetElement().Id();

	// If this were "16px" instead, the gap would stay a fixed pixel width no matter how far the
	// canvas rescaled the children arranged around it -- exactly what let a two-column grid overflow
	// its own container and collapse to one column at a small enough resolution.
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::ROW_GAP) == "16dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::COLUMN_GAP) == "16dp");
}

TEST_CASE("Padding is authored in dp, not px, so it scales with the canvas's UI scale factor")
{
	LayoutGroupFixture fixture;

	auto elementId = fixture.group->GetElement().Id();

	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::PADDING_LEFT) == "4dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::PADDING_RIGHT) == "4dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::PADDING_TOP) == "8dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::PADDING_BOTTOM) == "8dp");
}


TEST_SUITE_END();