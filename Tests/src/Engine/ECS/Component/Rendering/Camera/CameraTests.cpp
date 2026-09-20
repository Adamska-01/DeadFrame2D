#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Engine/Events/Context/Renderer/RenderTargetSizeChangedEvent.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Engine/ECS/System/Scene/TestScene.h"
#include <algorithm>
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct CameraFixture
	{
		EventDispatcher eventDispatcher;

		std::unique_ptr<Renderer> renderer;

		MockRenderBackend* renderBackend;

		std::shared_ptr<TestScene> scene;

		ObjectHandle<TestGameObject> owner;

		ComponentHandle<Camera> camera;


		CameraFixture()
		{
			auto rb = std::make_unique<MockRenderBackend>();
			renderBackend = rb.get();
			renderer = std::make_unique<Renderer>(std::move(rb));

			scene = std::make_shared<TestScene>(&eventDispatcher);
			scene->SetCoreContext(CoreContext{ .renderer = renderer.get() });

			owner = scene->Spawn<TestGameObject>();
			camera = owner->AddComponent<Camera>();
			camera->Init();
		}
	};
}


TEST_SUITE_BEGIN("Camera");


TEST_CASE("Init throws when CoreContext/ServiceContext has no EventDispatcher")
{
	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	auto camera = owner->AddComponent<Camera>();

	CHECK_THROWS_AS(camera->Init(), std::invalid_argument);
}

TEST_CASE("Default zoom is 1.0")
{
	CameraFixture fixture;

	CHECK(fixture.camera->GetZoom() == doctest::Approx(1.0f));
}

TEST_CASE("SetZoom clamps to a small positive minimum instead of going to zero or negative")
{
	CameraFixture fixture;

	fixture.camera->SetZoom(-5.0f);
	CHECK(fixture.camera->GetZoom() == doctest::Approx(0.01f));

	fixture.camera->SetZoom(0.0f);
	CHECK(fixture.camera->GetZoom() == doctest::Approx(0.01f));

	fixture.camera->SetZoom(3.5f);
	CHECK(fixture.camera->GetZoom() == doctest::Approx(3.5f));
}

TEST_CASE("SetViewport/GetViewport round-trip")
{
	CameraFixture fixture;

	auto viewport = RectF{ 0.25f, 0.1f, 0.5f, 0.75f };
	fixture.camera->SetViewport(viewport);

	CHECK(fixture.camera->GetViewport() == viewport);
}

TEST_CASE("Init creates a render target sized to the renderer's resolution target")
{
	CameraFixture fixture;

	CHECK(fixture.renderBackend->createRenderTargetCount == 1);
	CHECK(fixture.camera->GetRenderTarget() != 0);
	CHECK(fixture.renderBackend->lastRenderTargetSize == Vector2I{ 1920, 1080 });
}

TEST_CASE("GetViewBox reflects the renderer's resolution target")
{
	CameraFixture fixture;

	CHECK(fixture.camera->GetViewBox() == RectF{ 0.0f, 0.0f, 1920.0f, 1080.0f });
}

TEST_CASE("GetNormalizedViewBox scales the normalized viewport by the resolution target")
{
	CameraFixture fixture;

	fixture.camera->SetViewport(RectF{ 0.5f, 0.0f, 0.5f, 1.0f });

	CHECK(fixture.camera->GetNormalizedViewBox() == RectF{ 960.0f, 0.0f, 960.0f, 1080.0f });
}

TEST_CASE("A render-target-size-changed event updates the view box and recreates the render target")
{
	CameraFixture fixture;

	fixture.eventDispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 800, 600 }));

	CHECK(fixture.camera->GetViewBox() == RectF{ 0.0f, 0.0f, 800.0f, 600.0f });
	CHECK(fixture.renderBackend->createRenderTargetCount == 2);
	CHECK(fixture.renderBackend->destroyTextureCount == 1);
}

TEST_CASE("WorldToScreen centers the camera's own position on the resolution target's midpoint")
{
	CameraFixture fixture;

	// Camera's Transform is at the default world position (0,0), zoom 1.0.
	auto screenPos = fixture.camera->WorldToScreen(Vector2F::Zero);
	CHECK(screenPos.x == doctest::Approx(960.0f));
	CHECK(screenPos.y == doctest::Approx(540.0f));
}

TEST_CASE("ScreenToWorld is the inverse of WorldToScreen")
{
	CameraFixture fixture;

	auto worldPos = Vector2F{ 123.0f, -45.0f };
	auto screenPos = fixture.camera->WorldToScreen(worldPos);
	auto roundTripped = fixture.camera->ScreenToWorld(screenPos);

	CHECK(roundTripped.x == doctest::Approx(worldPos.x));
	CHECK(roundTripped.y == doctest::Approx(worldPos.y));
}

TEST_CASE("Default fit mode is ENVELOPE, which leaves the view scale at 1")
{
	CameraFixture fixture;

	CHECK(fixture.camera->GetFitMode() == CameraFitMode::ENVELOPE);
	CHECK(fixture.camera->GetViewScale() == doctest::Approx(1.0f));
}

TEST_CASE("ENVELOPE ignores the reference resolution entirely")
{
	CameraFixture fixture;

	fixture.camera->SetReferenceResolution(Vector2I{ 480, 270 });

	CHECK(fixture.camera->GetViewScale() == doctest::Approx(1.0f));
}

TEST_CASE("MATCH_WIDTH scales the view by the target's width over the reference width")
{
	CameraFixture fixture;

	// Render target is 1920x1080 (MockRenderBackend's default); a 960-wide reference doubles the scale.
	fixture.camera->SetReferenceResolution(Vector2I{ 960, 1080 });
	fixture.camera->SetFitMode(CameraFitMode::MATCH_WIDTH);

	CHECK(fixture.camera->GetViewScale() == doctest::Approx(2.0f));
}

TEST_CASE("MATCH_HEIGHT scales the view by the target's height over the reference height")
{
	CameraFixture fixture;

	fixture.camera->SetReferenceResolution(Vector2I{ 1920, 540 });
	fixture.camera->SetFitMode(CameraFitMode::MATCH_HEIGHT);

	CHECK(fixture.camera->GetViewScale() == doctest::Approx(2.0f));
}

TEST_CASE("LETTERBOX takes whichever axis would reveal less of the reference view")
{
	CameraFixture fixture;

	// Width ratio is 2, height ratio is 1: LETTERBOX must pick the smaller so the full reference
	// height still fits, at the cost of not filling the width.
	fixture.camera->SetReferenceResolution(Vector2I{ 960, 1080 });
	fixture.camera->SetFitMode(CameraFitMode::LETTERBOX);

	CHECK(fixture.camera->GetViewScale() == doctest::Approx(1.0f));
}

TEST_CASE("Setting the fit mode recomputes the view scale immediately")
{
	CameraFixture fixture;

	fixture.camera->SetReferenceResolution(Vector2I{ 960, 1080 });
	CHECK(fixture.camera->GetViewScale() == doctest::Approx(1.0f));

	fixture.camera->SetFitMode(CameraFitMode::MATCH_WIDTH);
	CHECK(fixture.camera->GetViewScale() == doctest::Approx(2.0f));
}

TEST_CASE("A render-target-size-changed event recomputes the view scale under a non-ENVELOPE fit mode")
{
	CameraFixture fixture;

	fixture.camera->SetReferenceResolution(Vector2I{ 1920, 1080 });
	fixture.camera->SetFitMode(CameraFitMode::MATCH_WIDTH);
	CHECK(fixture.camera->GetViewScale() == doctest::Approx(1.0f));

	fixture.eventDispatcher.SendEvent(std::make_shared<RenderTargetSizeChangedEvent>(Vector2I{ 3840, 2160 }));

	CHECK(fixture.camera->GetViewScale() == doctest::Approx(2.0f));
}

TEST_CASE("The view scale multiplies with zoom in WorldToScreen")
{
	CameraFixture fixture;

	fixture.camera->SetReferenceResolution(Vector2I{ 960, 1080 });
	fixture.camera->SetFitMode(CameraFitMode::MATCH_WIDTH);
	fixture.camera->SetZoom(2.0f);

	// Combined scale is zoom(2) * viewScale(2) = 4.
	auto screenPos = fixture.camera->WorldToScreen(Vector2F{ 10.0f, 0.0f });

	CHECK(screenPos.x == doctest::Approx(960.0f + 40.0f));
}

TEST_CASE("GetCameras includes a camera while it is alive")
{
	CameraFixture fixture;

	const auto& cameras = Camera::GetCameras();

	CHECK(std::find(cameras.begin(), cameras.end(), fixture.camera) != cameras.end());
}


TEST_SUITE_END();