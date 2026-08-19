#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Input/Input.h"
#include "Core/Services/Events/Abstractions/ISystemEventSink.h"
#include "Data/Blueprints/UI/Button/ButtonComponentModel.h"
#include "Data/Systems/Events/SystemEvent.h"
#include "Models/Input/ActionMap/InputActionMapBucket.h"
#include "Engine/ECS/Component/UI/Button.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/System/Events/EventDispatcher.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Mocks/Engine/ECS/Entity/DummyGameComponent.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include <doctest.h>
#include <memory>
#include <stdexcept>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;
using namespace DF2D::Models;


namespace
{
	struct ButtonFixture
	{
		EventDispatcher dispatcher;

		std::unique_ptr<Input> input;

		std::unique_ptr<TextureManager> textureManager;

		MockTextureBackend* backend;

		std::shared_ptr<FakeSceneHandleProvider> scene;

		ObjectHandle<TestGameObject> canvasOwner;

		ObjectHandle<TestGameObject> owner;


		ButtonFixture()
		{
			input = std::make_unique<Input>(InputActionMapBucket{}, dispatcher);

			auto mock = std::make_unique<MockTextureBackend>();
			backend = mock.get();
			textureManager = std::make_unique<TextureManager>(std::move(mock));

			scene = std::make_shared<FakeSceneHandleProvider>();
			scene->SetCoreContext(CoreContext{ .textureManager = textureManager.get(), .input = input.get() });

			canvasOwner = scene->Create<TestGameObject>();
			canvasOwner->AddComponent<Canvas>();

			owner = scene->Create<TestGameObject>();
			owner->SetParent(canvasOwner);
		}

		ISystemEventSink& Sink()
		{
			return *input;
		}
	};
}


TEST_SUITE_BEGIN("Button");


TEST_CASE("Init with empty sources does not touch the texture backend")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{});
	button->Init();

	CHECK(fixture.backend->loadCount == 0);
	CHECK(button->GetCurrentButtonImage() == 0);
}

TEST_CASE("Init loads every configured source and starts on the idle image")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{
		.idleButtonSource = "idle.png",
		.hoveredButtonSource = "hover.png",
		.pressedButtonSource = "press.png" });
	button->Init();

	CHECK(fixture.backend->loadCount == 3);
	CHECK(button->GetCurrentButtonImage() != 0);
}

TEST_CASE("GetBoundingBox centers the widget size on the transform position")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{ .buttonSize = Vector2F(100.0f, 50.0f) });
	button->Init();

	auto box = button->GetBoundingBox();

	CHECK(box.x == doctest::Approx(-50.0f));
	CHECK(box.y == doctest::Approx(-25.0f));
	CHECK(box.w == doctest::Approx(100.0f));
	CHECK(box.h == doctest::Approx(50.0f));
}

TEST_CASE("Moving the mouse over the button triggers OnPointerEnter and fires the enter callback")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{ .buttonSize = Vector2F(100.0f, 50.0f) });
	button->Init();

	auto handleOwner = fixture.owner->AddComponent<DummyGameComponent>();
	auto entered = false;
	button->AddEnterCallback(handleOwner, [&]() { entered = true; });

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 0.0f, 0.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);

	CHECK(button->IsHovered());
	CHECK(entered);
}

TEST_CASE("Moving the mouse away triggers OnPointerExit")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{ .buttonSize = Vector2F(100.0f, 50.0f) });
	button->Init();

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 0.0f, 0.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);
	REQUIRE(button->IsHovered());

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 1000.0f, 1000.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);

	CHECK_FALSE(button->IsHovered());
}

TEST_CASE("Press then release while hovered fires the pressed callback")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{ .buttonSize = Vector2F(100.0f, 50.0f) });
	button->Init();

	auto handleOwner = fixture.owner->AddComponent<DummyGameComponent>();
	auto pressed = false;
	button->AddPressedCallback(handleOwner, [&]() { pressed = true; });

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 0.0f, 0.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);

	fixture.Sink().OnSystemEvent(MouseButtonEvent{ MouseButtonCode::LEFT, true, { 0.0f, 0.0f } });
	button->Update(0.016f);
	CHECK(button->IsPressed());
	CHECK_FALSE(pressed);

	fixture.Sink().OnSystemEvent(MouseButtonEvent{ MouseButtonCode::LEFT, false, { 0.0f, 0.0f } });
	button->Update(0.016f);

	CHECK(pressed);
	CHECK_FALSE(button->IsPressed());
}

TEST_CASE("Releasing outside the button after a press does not fire the pressed callback")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{ .buttonSize = Vector2F(100.0f, 50.0f) });
	button->Init();

	auto handleOwner = fixture.owner->AddComponent<DummyGameComponent>();
	auto pressed = false;
	button->AddPressedCallback(handleOwner, [&]() { pressed = true; });

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 0.0f, 0.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);

	fixture.Sink().OnSystemEvent(MouseButtonEvent{ MouseButtonCode::LEFT, true, { 0.0f, 0.0f } });
	button->Update(0.016f);

	fixture.Sink().OnSystemEvent(MouseMoveEvent{ { 1000.0f, 1000.0f }, { 1.0f, 0.0f } });
	button->Update(0.016f);

	fixture.Sink().OnSystemEvent(MouseButtonEvent{ MouseButtonCode::LEFT, false, { 1000.0f, 1000.0f } });
	button->Update(0.016f);

	CHECK_FALSE(pressed);
	CHECK_FALSE(button->IsPressed());
}

TEST_CASE("Draw does not throw once parented under a Canvas")
{
	ButtonFixture fixture;

	auto button = fixture.owner->AddComponent<Button>(ButtonComponentModel{});
	button->Init();
	button->Update(0.016f);

	CHECK_NOTHROW(button->Draw());
}

TEST_CASE("Update throws when there is no Canvas ancestor")
{
	ButtonFixture fixture;

	auto orphan = fixture.scene->Create<TestGameObject>();
	auto button = orphan->AddComponent<Button>(ButtonComponentModel{});
	button->Init();

	CHECK_THROWS_AS(button->Update(0.016f), std::invalid_argument);
}


TEST_SUITE_END();