#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Systems/UI/UIStyleProperty.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/RectTransform.h"
#include "Engine/ECS/Component/UI/Text.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include "Mocks/Context/Systems/UI/MockUIBackend.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <cmath>
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	// A canvas needs a renderer for its resolution and a UI manager for its context; nothing else in
	// these tests touches the rest of the core context.
	struct CanvasScaleFixture
	{
		EventDispatcher dispatcher;

		std::unique_ptr<Renderer> renderer;

		MockTimeProvider time;

		MockUIBackend* mock = nullptr;

		std::unique_ptr<UIManager> uiManager;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> canvasObject;

		ComponentHandle<Canvas> canvas;


		CanvasScaleFixture()
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

			scene->Init();
		}
	};
}


TEST_SUITE_BEGIN("Canvas");


TEST_CASE("Default UI scale mode is SCALE_WITH_SCREEN_SIZE with a 1920x1080 reference")
{
	CanvasScaleFixture fixture;

	CHECK(fixture.canvas->GetUIScaleMode() == UIScaleMode::SCALE_WITH_SCREEN_SIZE);
	CHECK(fixture.canvas->GetReferenceResolution() == Vector2I{ 1920, 1080 });
}

TEST_CASE("The scale factor is 1 when the render target already matches the reference resolution")
{
	// MockRenderBackend defaults its resolution target to 1920x1080, matching Canvas's own default
	// reference resolution, so the two start out already in agreement.
	CanvasScaleFixture fixture;

	CHECK(fixture.canvas->GetUIScaleFactor() == doctest::Approx(1.0f));
}

TEST_CASE("CONSTANT_PIXEL_SIZE ignores the reference resolution entirely")
{
	CanvasScaleFixture fixture;

	fixture.canvas->SetUIScaleMode(UIScaleMode::CONSTANT_PIXEL_SIZE);
	fixture.canvas->SetReferenceResolution(Vector2I{ 480, 270 });

	CHECK(fixture.canvas->GetUIScaleFactor() == doctest::Approx(1.0f));
}

TEST_CASE("A render-target-size-changed event rescales dp against the reference resolution")
{
	CanvasScaleFixture fixture;

	// Half the reference resolution on both axes, so both the width and height ratios agree: the
	// blend between them (matchWidthOrHeight) cannot change the result either way.
	fixture.dispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 960, 540 }));

	CHECK(fixture.canvas->GetUIScaleFactor() == doctest::Approx(0.5f));
}

TEST_CASE("matchWidthOrHeight blends the width and height ratios in log space")
{
	CanvasScaleFixture fixture;

	// Width ratio 2 (3840/1920), height ratio 1 (1080/1080). At the default 0.5 blend, the scale is
	// the geometric mean of the two ratios: sqrt(2*1).
	fixture.dispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 3840, 1080 }));

	CHECK(fixture.canvas->GetUIScaleFactor() == doctest::Approx(std::sqrt(2.0f)));
}

TEST_CASE("SetMatchWidthOrHeight of 0 follows the width ratio only")
{
	CanvasScaleFixture fixture;

	fixture.canvas->SetMatchWidthOrHeight(0.0f);
	fixture.dispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 3840, 1080 }));

	CHECK(fixture.canvas->GetUIScaleFactor() == doctest::Approx(2.0f));
}

TEST_CASE("A resize propagates the new scale factor to every RectTransform under the canvas")
{
	CanvasScaleFixture fixture;

	auto child = fixture.scene->Spawn<TestGameObject>();
	child->SetParent(fixture.canvasObject);

	auto rectTransform = child->AddComponent<RectTransform>();
	rectTransform->SetSizeDelta(Vector2F{ 100.0f, 100.0f });

	fixture.scene->Init();

	fixture.dispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 960, 540 }));

	// 100dp at half scale resolves to 50px; if the resize hadn't reached this element it would still
	// carry whatever width it resolved to before the scale factor changed.
	auto elementId = rectTransform->GetElement().Id();

	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::WIDTH) == "50px");
}

TEST_CASE("A resize also rescales the context's dp ratio, so stylesheet-authored lengths track the same factor as RectTransform-driven ones")
{
	CanvasScaleFixture fixture;

	fixture.dispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 960, 540 }));

	auto contextId = fixture.canvas->GetContext().Id();

	// Without this, a font-size or border-width authored in a stylesheet would stay a fixed pixel
	// count no matter how far the canvas rescaled everything else, oversized against shrunken boxes.
	CHECK(fixture.mock->densityRatios.at(contextId) == doctest::Approx(0.5f));
}

TEST_CASE("Text authors its font size in dp, so it scales along with the context's density ratio")
{
	CanvasScaleFixture fixture;

	auto child = fixture.scene->Spawn<TestGameObject>();
	child->SetParent(fixture.canvasObject);

	auto text = child->AddComponent<Text>();
	text->SetFontSize(18.0f);

	fixture.scene->Init();

	// If this were "18px" instead, RecomputeUIScaleFactor's density-ratio call would have nothing to
	// act on: text would stay a fixed pixel size no matter how far the canvas rescaled everything else.
	auto elementId = text->GetElement().Id();

	CHECK(fixture.mock->PropertyOf(elementId, UIStyleProperty::FONT_SIZE) == "18dp");
}


TEST_SUITE_END();