#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Layout/LayoutElement.h"
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
	struct LayoutElementFixture
	{
		EventDispatcher dispatcher;

		std::unique_ptr<Renderer> renderer;

		MockTimeProvider time;

		MockUIBackend* mock = nullptr;

		std::unique_ptr<UIManager> uiManager;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> canvasObject;

		ComponentHandle<Canvas> canvas;

		ObjectHandle<TestGameObject> elementObject;

		ComponentHandle<LayoutElement> layoutElement;


		LayoutElementFixture()
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

			elementObject = scene->Spawn<TestGameObject>();
			elementObject->SetParent(canvasObject);
			layoutElement = elementObject->AddComponent<LayoutElement>();
			layoutElement->SetMinSize(Vector2F{ 10.0f, 20.0f });
			layoutElement->SetPreferredSize(Vector2F{ 120.0f, 40.0f });

			scene->Init();
		}
	};
}


TEST_SUITE_BEGIN("LayoutElement");


TEST_CASE("Min and preferred sizes are authored in dp, not px, so they scale with the canvas's UI scale factor")
{
	LayoutElementFixture fixture;

	auto elementId = fixture.layoutElement->GetElement().Id();

	// If these were px instead, a LayoutElement's size would stay fixed no matter how far the canvas
	// rescaled everything else arranged around it -- the same bug LayoutGroup's spacing/padding had.
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::MIN_WIDTH) == "10dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::MIN_HEIGHT) == "20dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::WIDTH) == "120dp");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::HEIGHT) == "40dp");
}

TEST_CASE("A negative preferred size axis leaves that axis to content instead of writing a dp value")
{
	LayoutElementFixture fixture;

	fixture.layoutElement->SetPreferredSize(Vector2F{ -1.0f, -1.0f });

	auto elementId = fixture.layoutElement->GetElement().Id();

	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::WIDTH) == "auto");
	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::HEIGHT) == "auto");
}


TEST_SUITE_END();
