#include "Constants/Rendering/DefaultSortOrders.h"
#include "Data/Components/UI/Canvas/CanvasRenderMode.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>


using namespace DF2D::Constants;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct CanvasFixture
	{
		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> owner;

		ComponentHandle<Canvas> canvas;


		CanvasFixture()
		{
			scene = std::make_shared<FakeSceneHandleProvider>();
			owner = scene->Create<TestGameObject>();
			canvas = owner->AddComponent<Canvas>();
		}
	};
}


TEST_SUITE_BEGIN("Canvas");


TEST_CASE("Default sort order is DefaultSortOrders::UI_RENDERER")
{
	CanvasFixture fixture;

	CHECK(fixture.canvas->GetSortOrder() == DefaultSortOrders::UI_RENDERER);
}

TEST_CASE("Default render mode is SCREEN_SPACE_OVERLAY")
{
	CanvasFixture fixture;

	CHECK(fixture.canvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_OVERLAY);
}

TEST_CASE("Default render camera is null")
{
	CanvasFixture fixture;

	CHECK(fixture.canvas->GetRenderCamera() == nullptr);
}

TEST_CASE("SetSortOrder/GetSortOrder round-trip")
{
	CanvasFixture fixture;

	fixture.canvas->SetSortOrder(42);

	CHECK(fixture.canvas->GetSortOrder() == 42);
}

TEST_CASE("SetScreenSpaceCamera with a valid camera switches to camera mode and stores it")
{
	CanvasFixture fixture;

	auto camera = fixture.owner->AddComponent<Camera>();
	fixture.canvas->SetScreenSpaceCamera(camera);

	CHECK(fixture.canvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_CAMERA);
	CHECK(fixture.canvas->GetRenderCamera() == camera);
}

TEST_CASE("SetScreenSpaceCamera with a null camera falls back to overlay mode")
{
	CanvasFixture fixture;

	fixture.canvas->SetScreenSpaceCamera(ComponentHandle<Camera>());

	CHECK(fixture.canvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_OVERLAY);
	CHECK(fixture.canvas->GetRenderCamera() == nullptr);
}

TEST_CASE("SetScreenSpaceOverlay clears a previously set camera")
{
	CanvasFixture fixture;

	auto camera = fixture.owner->AddComponent<Camera>();
	fixture.canvas->SetScreenSpaceCamera(camera);

	fixture.canvas->SetScreenSpaceOverlay();

	CHECK(fixture.canvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_OVERLAY);
	CHECK(fixture.canvas->GetRenderCamera() == nullptr);
}


TEST_SUITE_END();