#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Component/UI/Button.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/Image.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Text.h"
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
	struct CanvasFixture
	{
		EventDispatcher dispatcher;

		std::unique_ptr<Renderer> renderer;

		MockTimeProvider time;

		MockUIBackend* mock = nullptr;

		std::unique_ptr<UIManager> uiManager;

		std::shared_ptr<TestScene> scene;


		CanvasFixture()
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
		}
	};
}


TEST_SUITE_BEGIN("Canvas");


TEST_CASE("A scene holding a canvas and UI components tears down cleanly")
{
	auto fixture = CanvasFixture();

	auto canvasObject = fixture.scene->Spawn<TestGameObject>();
	canvasObject->AddComponent<Canvas>();

	auto panel = fixture.scene->Spawn<TestGameObject>();
	panel->SetParent(canvasObject);
	panel->AddComponent<RectTransform>();
	panel->AddComponent<Image>();

	auto button = fixture.scene->Spawn<TestGameObject>();
	button->SetParent(canvasObject);
	button->AddComponent<RectTransform>();
	button->AddComponent<Button>();

	fixture.scene->Init();

	// This is what a scene change does: the canvas is destroyed alongside everything under it. The
	// canvas frees its whole context, so the child components must not then reach for elements the
	// context has already taken with it.
	CHECK_NOTHROW(fixture.scene.reset());
}


TEST_CASE("Destroying the canvas object while its children live does not strand them")
{
	auto fixture = CanvasFixture();

	auto canvasObject = fixture.scene->Spawn<TestGameObject>();
	canvasObject->AddComponent<Canvas>();

	auto child = fixture.scene->Spawn<TestGameObject>();
	child->SetParent(canvasObject);
	child->AddComponent<RectTransform>();
	child->AddComponent<Text>();

	fixture.scene->Init();

	CHECK_NOTHROW(canvasObject->Destroy());
	CHECK_NOTHROW(fixture.scene->Update(0.016f));
}


TEST_CASE("A destroyed canvas takes its surface with it and stops being updated")
{
	auto fixture = CanvasFixture();

	auto canvasObject = fixture.scene->Spawn<TestGameObject>();
	canvasObject->AddComponent<Canvas>();

	fixture.scene->Init();

	CHECK(fixture.mock->createContextCount == 1);

	canvasObject->Destroy();
	fixture.scene->Update(0.016f);

	static_cast<DF2D::Core::ICoreSystem*>(fixture.uiManager.get())->EndUpdate(0.016f);

	// Destroying the surface is the canvas's job alone, which is why UIContext::Destroy is private to
	// it. What matters here is that the manager stops treating a dead surface as live.
	CHECK(fixture.mock->destroyContextCount == 1);
	CHECK(fixture.mock->updateContextCount == 0);
}


TEST_SUITE_END();