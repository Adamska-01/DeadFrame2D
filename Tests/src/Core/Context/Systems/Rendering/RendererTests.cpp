#include <doctest.h>
#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Mocks/Context/Systems/Rendering/MockRenderBackend.h"


using namespace DF2D::Core;


static std::unique_ptr<Renderer> MakeRenderer(MockRenderBackend*& outMock)
{
	auto mock = std::make_unique<MockRenderBackend>();
	outMock = mock.get();

	return std::make_unique<Renderer>(std::move(mock));
}


TEST_SUITE_BEGIN("Renderer");


TEST_CASE("Constructor succeeds with valid backend")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	CHECK(mock->createRenderTargetCount == 0);
	CHECK(mock->presentCount == 0);
}


TEST_CASE("CreateRenderTarget delegates size and returns backend id")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	mock->nextRenderTargetId = 42;

	auto id = renderer->CreateRenderTarget(320, 240);

	CHECK(id == 42);
	CHECK(mock->createRenderTargetCount == 1);
	CHECK(mock->lastRenderTargetSize.x == 320);
	CHECK(mock->lastRenderTargetSize.y == 240);
}


TEST_CASE("DestroyTexture delegates id to backend")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	renderer->DestroyTexture(7);

	CHECK(mock->destroyTextureCount == 1);
	CHECK(mock->lastDestroyedTexture == 7);
}


TEST_CASE("SetDisplayColor forwards components and GetDisplayColor reads them back")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	renderer->SetDisplayColor(10, 20, 30, 40);

	auto color = renderer->GetDisplayColor();

	CHECK(color == Color{10, 20, 30, 40});
}


TEST_CASE("SetResolutionTarget forwards and GetResolutionTarget reads back")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	renderer->SetResolutionTarget({640, 480});

	auto resolution = renderer->GetResolutionTarget();

	CHECK(resolution.x == 640);
	CHECK(resolution.y == 480);
}


TEST_CASE("SetViewport delegates rect to backend")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	renderer->SetViewport({1, 2, 300, 400});

	CHECK(mock->lastViewport.x == 1);
	CHECK(mock->lastViewport.y == 2);
	CHECK(mock->lastViewport.w == 300);
	CHECK(mock->lastViewport.h == 400);
}


TEST_CASE("ICoreSystem lifecycle methods are safe no-ops")
{
	MockRenderBackend* mock = nullptr;
	auto renderer = MakeRenderer(mock);

	ICoreSystem* system = renderer.get();

	system->BeginFrame();
	system->PreUpdate(0.0f);
	system->EndUpdate(0.0f);
	system->EndDraw();

	CHECK(mock->presentCount == 0);
	CHECK(mock->drawCount == 0);
}


TEST_SUITE_END();
