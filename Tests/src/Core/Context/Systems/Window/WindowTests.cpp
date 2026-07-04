#include <doctest.h>
#include "Core/Context/Systems/Window/Window.h"
#include "Mocks/Context/Systems/Window/MockWindowBackend.h"
#include <SDL.h>


using namespace DF2D::Core;
using namespace DF2D::Data;


static std::unique_ptr<Window> MakeWindow(MockWindowBackend*& outMock)
{
	auto mock = std::make_unique<MockWindowBackend>();
	outMock = mock.get();

	return std::make_unique<Window>(std::move(mock));
}


TEST_SUITE_BEGIN("Window");


TEST_CASE("GetResolution delegates to backend")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	mock->resolution = {800, 600};

	auto resolution = window->GetResolution();

	CHECK(resolution.x == 800);
	CHECK(resolution.y == 600);
}


TEST_CASE("SetWindowTitle delegates to backend")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	window->SetWindowTitle("My Game");

	CHECK(mock->lastTitle == "My Game");
}


TEST_CASE("SetWindowIcon delegates to backend")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	window->SetWindowIcon("icon.png");

	CHECK(mock->lastIconSource == "icon.png");
}


TEST_CASE("SetWindowMode forwards mode enum unchanged")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	window->SetWindowMode(WindowMode::BORDERLESS);
	CHECK(mock->lastMode == WindowMode::BORDERLESS);

	window->SetWindowMode(WindowMode::FULLSCREEN);
	CHECK(mock->lastMode == WindowMode::FULLSCREEN);
}


TEST_CASE("SetResolution propagates backend success")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	auto result = window->SetResolution({1280, 720});

	CHECK(result == true);
	CHECK(mock->setResolutionCount == 1);
	CHECK(mock->lastRequestedResolution.x == 1280);
	CHECK(mock->lastRequestedResolution.y == 720);
}


TEST_CASE("SetResolution propagates backend failure")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	mock->setResolutionResult = false;

	auto result = window->SetResolution({123, 456});

	CHECK(result == false);
	CHECK(mock->setResolutionCount == 1);
}


TEST_CASE("ProcessEvents returns SDL_QUIT code for quit events")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	IEventProcessor* processor = window.get();

	SDL_Event quitEvent{};
	quitEvent.type = SDL_QUIT;

	auto result = processor->ProcessEvents(quitEvent);

	REQUIRE(result.has_value());
	CHECK(*result == (int)SDL_QUIT);
}


TEST_CASE("ProcessEvents ignores unrelated events")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	IEventProcessor* processor = window.get();

	SDL_Event keyEvent{};
	keyEvent.type = SDL_KEYDOWN;

	auto result = processor->ProcessEvents(keyEvent);

	CHECK_FALSE(result.has_value());
}


TEST_CASE("ICoreSystem lifecycle methods are safe no-ops")
{
	MockWindowBackend* mock = nullptr;
	auto window = MakeWindow(mock);

	ICoreSystem* system = window.get();

	system->BeginFrame();
	system->PreUpdate(0.0f);
	system->EndUpdate(0.0f);
	system->EndDraw();

	CHECK(mock->setResolutionCount == 0);
}


TEST_SUITE_END();
