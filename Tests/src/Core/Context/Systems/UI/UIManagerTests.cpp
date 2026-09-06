#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Mocks/Context/Systems/UI/MockUIBackend.h"
#include "Mocks/Engine/ECS/Entity/FakeSceneHandleProvider.h"
#include "Mocks/Engine/ECS/Entity/TestGameObject.h"
#include "Mocks/Services/Time/MockTimeProvider.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;
using namespace DF2D::Engine;


namespace
{
	struct Fixture
	{
		MockUIBackend* mock = nullptr;

		MockTimeProvider time;

		std::unique_ptr<UIManager> manager;


		/** @brief Fonts already registered by the constructor, so tests can measure their own deltas. */
		size_t baselineFontCount = 0;


		/** @brief A surface made on demand, so tests that never ask for one still count zero. */
		UIContext context;


		/** @brief A real element: owner registration is reached through the element that owns it. */
		UIElement ElementFor(const DF2D::Engine::ObjectHandle<DF2D::Engine::GameObject>& owner)
		{
			if (!context.IsValid())
			{
				context = manager->CreateCanvasContext(Vector2I(800, 600));
			}

			return context.AcquireElement(owner);
		}


		Fixture()
		{
			auto owned = std::make_unique<MockUIBackend>();

			mock = owned.get();
			manager = std::make_unique<UIManager>(std::move(owned), &time);

			baselineFontCount = mock->loadedFonts.size();
		}
	};


	std::shared_ptr<FakeSceneHandleProvider> objectScene = std::make_shared<FakeSceneHandleProvider>();

	ObjectHandle<GameObject> firstObject = objectScene->Create<TestGameObject>();
	ObjectHandle<GameObject> secondObject = objectScene->Create<TestGameObject>();

	/** @brief Minimal UI component that records the events routed to it. */
	struct TestUIComponent : DF2D::Engine::UIComponent
	{
		TYPE_INFO(TestUIComponent, UIComponent);


	public:
		int eventCount = 0;


	protected:
		void HandleUIEvent(UIEventType, const UIEventPayload&) override
		{
			eventCount++;
		}
	};


	// Components live in a real ComponentBucket so their handles are valid, and can be removed to
	// invalidate them, mirroring how PhysicsEngine2DTests exercises ContactEventProvider handles.
	DF2D::Engine::ComponentHandle<TestUIComponent> MakeComponent(std::shared_ptr<DF2D::Engine::ComponentBucket>& bucket)
	{
		if (bucket == nullptr)
		{
			bucket = std::make_shared<DF2D::Engine::ComponentBucket>();
		}

		return bucket->AddComponent<TestUIComponent>(DF2D::Engine::ObjectHandle<DF2D::Engine::GameObject>{});
	}

	void Dispatch(UIManager& manager, UIElementID element)
	{
		static_cast<IUIEventSink*>(&manager)->OnUIEvent(element, UIEventType::CLICK, UIEventPayload{});
	}

	void Feed(UIManager& manager, const DF2D::Data::SystemEvent& systemEvent)
	{
		static_cast<DF2D::Core::ISystemEventSink*>(&manager)->OnSystemEvent(systemEvent);
	}
}


TEST_SUITE_BEGIN("UIManager");


TEST_CASE("The manager registers itself as the backend event sink")
{
	auto fixture = Fixture();

	CHECK(fixture.mock->sink != nullptr);
}


TEST_CASE("The constructor registers a default face under the engine font family")
{
	auto fixture = Fixture();

	// The default stylesheet names a family, so something has to supply it before any text renders.
	REQUIRE(fixture.mock->loadedFonts.size() == 1);
	CHECK(fixture.mock->loadedFontFamilies[0] == "DeadFrame");
}


TEST_CASE("LoadFont delegates to the backend")
{
	auto fixture = Fixture();

	CHECK(fixture.manager->LoadFont("Fonts/other.ttf"));
	CHECK(fixture.mock->loadedFonts.size() == fixture.baselineFontCount + 1);
	CHECK(fixture.mock->loadedFonts.back() == "Fonts/other.ttf");
}


TEST_CASE("Loading the same font twice only reaches the backend once")
{
	auto fixture = Fixture();

	fixture.manager->LoadFont("Fonts/other.ttf");
	fixture.manager->LoadFont("Fonts/other.ttf");

	CHECK(fixture.mock->loadedFonts.size() == fixture.baselineFontCount + 1);
}


TEST_CASE("A failed font load is not cached, so a later attempt retries")
{
	auto fixture = Fixture();

	fixture.mock->failNextFontLoad = true;

	CHECK_FALSE(fixture.manager->LoadFont("Fonts/missing.ttf"));

	fixture.mock->failNextFontLoad = false;

	CHECK(fixture.manager->LoadFont("Fonts/missing.ttf"));
	CHECK(fixture.mock->loadedFonts.size() == fixture.baselineFontCount + 2);
}


TEST_CASE("Creating a canvas surface yields an inert handle when the backend fails")
{
	auto fixture = Fixture();

	fixture.mock->failNextContext = true;

	CHECK(fixture.manager->CreateCanvasContext(Vector2I(800, 600)).IsValid() == false);
}


TEST_CASE("Live contexts are updated once per frame")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));
	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	static_cast<ICoreSystem*>(fixture.manager.get())->EndUpdate(0.016f);

	CHECK(fixture.mock->updateContextCount == 2);
}


TEST_CASE("The UI clock advances every frame")
{
	auto fixture = Fixture();

	fixture.time.deltaTimeUnscaled = 0.016f;

	auto* system = static_cast<ICoreSystem*>(fixture.manager.get());

	system->EndUpdate(0.016f);
	system->EndUpdate(0.016f);
	system->EndUpdate(0.016f);

	// A clock that stops moving strands anything the backend animates against it: a smooth scroll
	// freezes one frame in, and every further wheel notch piles onto a target it can never reach.
	CHECK(fixture.mock->advanceCount == 3);
	CHECK(fixture.mock->elapsedTime == doctest::Approx(0.048f));
}


TEST_CASE("The UI clock keeps moving while gameplay is paused")
{
	auto fixture = Fixture();

	// What a paused game looks like: gameplay sees nothing, the wall clock carries on.
	fixture.time.timeScale = 0.0f;
	fixture.time.deltaTime = 0.0f;
	fixture.time.deltaTimeUnscaled = 0.016f;

	static_cast<ICoreSystem*>(fixture.manager.get())->EndUpdate(0.0f);

	CHECK(fixture.mock->lastAdvanceDelta == doctest::Approx(0.016f));
}


TEST_CASE("The clock is advanced before layout is resolved")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	fixture.time.deltaTimeUnscaled = 0.016f;

	static_cast<ICoreSystem*>(fixture.manager.get())->EndUpdate(0.016f);

	// Layout reads whatever the clock-driven animations left behind, so the order is not incidental.
	CHECK(fixture.mock->updateContextCount == 1);
	CHECK(fixture.mock->advanceCountAtLastUpdateContext == 1);
}


TEST_CASE("Two UI components on one object share a single element")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	auto first = context.AcquireElement(firstObject);
	auto second = context.AcquireElement(firstObject);

	CHECK(first == second);
	CHECK(fixture.mock->createElementCount == 1);
}


TEST_CASE("Different objects get different elements")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	auto first = context.AcquireElement(firstObject);
	auto second = context.AcquireElement(secondObject);

	CHECK(first != second);
	CHECK(fixture.mock->createElementCount == 2);
}


TEST_CASE("A shared element survives until its last holder releases it")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	context.AcquireElement(firstObject);
	context.AcquireElement(firstObject);

	context.ReleaseElement(firstObject);

	CHECK(fixture.mock->destroyElementCount == 0);

	context.ReleaseElement(firstObject);

	CHECK(fixture.mock->destroyElementCount == 1);
}


TEST_CASE("Releasing an object that never acquired anything is a no-op")
{
	auto fixture = Fixture();

	fixture.ElementFor(firstObject);
	fixture.context.ReleaseElement(secondObject);

	CHECK(fixture.mock->destroyElementCount == 0);
}


TEST_CASE("An inert surface handle acquires nothing")
{
	auto fixture = Fixture();

	// An inert surface handle reaches nothing: the guard is the handle itself, not a check at each site.
	CHECK(UIContext().AcquireElement(firstObject).IsValid() == false);
	CHECK(fixture.mock->createElementCount == 0);
}


TEST_CASE("An inert surface handle renders nothing without reaching the backend")
{
	auto fixture = Fixture();

	auto drawList = UIContext().Render();

	CHECK(drawList.commands.empty());
	CHECK(fixture.mock->renderContextCount == 0);
}


TEST_CASE("Every component sharing an element receives its events")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto first = MakeComponent(bucket);
	auto second = MakeComponent(bucket);

	// One element, several components: a RectTransform, an Image and a Text on one object all sit on
	// the same element, so a single owner would silently starve all but the last registered.
	fixture.ElementFor(firstObject).RegisterOwner(first);
	fixture.ElementFor(firstObject).RegisterOwner(second);

	Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id());

	CHECK(first->eventCount == 1);
	CHECK(second->eventCount == 1);
}


TEST_CASE("A destroyed component is skipped while its co-owners still receive events")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto first = MakeComponent(bucket);
	auto second = MakeComponent(bucket);

	fixture.ElementFor(firstObject).RegisterOwner(first);
	fixture.ElementFor(firstObject).RegisterOwner(second);

	// Destroyed without unregistering, which is what a destructor path that skips cleanup leaves
	// behind. The handle expires, so dispatch must drop it rather than dereference it.
	bucket->RemoveComponent(first);

	CHECK_NOTHROW(Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id()));
	CHECK(second->eventCount == 1);
}


TEST_CASE("Unregistering one component leaves the others subscribed")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto first = MakeComponent(bucket);
	auto second = MakeComponent(bucket);

	fixture.ElementFor(firstObject).RegisterOwner(first);
	fixture.ElementFor(firstObject).RegisterOwner(second);

	fixture.ElementFor(firstObject).UnregisterOwner(first);

	Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id());

	CHECK(first->eventCount == 0);
	CHECK(second->eventCount == 1);
}


TEST_CASE("An element with no remaining owners stops dispatching")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto only = MakeComponent(bucket);

	fixture.ElementFor(firstObject).RegisterOwner(only);
	fixture.ElementFor(firstObject).UnregisterOwner(only);

	CHECK_NOTHROW(Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id()));
	CHECK(only->eventCount == 0);
}


TEST_CASE("Pointer motion is fed to every live context")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));
	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	Feed(*fixture.manager, DF2D::Data::MouseMoveEvent{ .position = { 12.0f, 34.0f } });

	CHECK(fixture.mock->mouseMoveCount == 2);
	CHECK(fixture.mock->lastPointerPosition == Vector2F(12.0f, 34.0f));
}


TEST_CASE("Pointer events reach the backend regardless of what it reports")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	// Nothing is withheld at this level any more: the devices must always be fed so their recorded
	// state stays truthful. Suppression happens later, when actions are resolved.
	Feed(*fixture.manager, DF2D::Data::MouseButtonEvent{});

	CHECK(fixture.mock->mouseButtonCount == 1);
}


TEST_CASE("Key and text events are routed separately from pointer events")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	Feed(*fixture.manager, DF2D::Data::KeyEvent{ .key = DF2D::Models::KeyboardKeyCode::A, .pressed = true });
	Feed(*fixture.manager, DF2D::Data::TextInputEvent{ .text = "a" });

	CHECK(fixture.mock->keyCount == 1);
	CHECK(fixture.mock->lastKey == DF2D::Models::KeyboardKeyCode::A);
	CHECK(fixture.mock->textInputCount == 1);
	CHECK(fixture.mock->lastText == "a");
}


TEST_CASE("Nothing is routed anywhere when no canvas exists")
{
	auto fixture = Fixture();

	Feed(*fixture.manager, DF2D::Data::MouseMoveEvent{});

	CHECK(fixture.mock->mouseMoveCount == 0);
}


TEST_CASE("Capture state is reported to the input system from the backend")
{
	auto fixture = Fixture();

	fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	// Reached through the interface the input system actually holds, not a second public accessor.
	const auto& capture = static_cast<const DF2D::Core::IInputCaptureState&>(*fixture.manager);

	CHECK_FALSE(capture.CapturesPointer());
	CHECK_FALSE(capture.CapturesKeyboard());

	fixture.mock->pointerOverElement = true;
	fixture.mock->keyboardFocused = true;

	CHECK(capture.CapturesPointer());
	CHECK(capture.CapturesKeyboard());
}


TEST_CASE("A widget's element kind wins regardless of which component declared first")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	// A positioning component declares first and has no opinion; the widget declares afterwards.
	context.DeclareElementType(firstObject, UIElementType::PANEL);
	context.DeclareElementType(firstObject, UIElementType::BUTTON);

	context.AcquireElement(firstObject);

	CHECK(fixture.mock->lastCreatedType == UIElementType::BUTTON);
}


TEST_CASE("Declaration order does not matter")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	// The reverse order of the previous case: the widget is initialised before its RectTransform.
	context.DeclareElementType(firstObject, UIElementType::BUTTON);
	context.DeclareElementType(firstObject, UIElementType::PANEL);

	context.AcquireElement(firstObject);

	CHECK(fixture.mock->lastCreatedType == UIElementType::BUTTON);
}


TEST_CASE("An object nobody has an opinion about gets a plain panel")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	context.DeclareElementType(firstObject, UIElementType::PANEL);
	context.AcquireElement(firstObject);

	CHECK(fixture.mock->lastCreatedType == UIElementType::PANEL);
}


TEST_CASE("Two conflicting widget kinds keep the first and warn")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	context.DeclareElementType(firstObject, UIElementType::BUTTON);
	context.DeclareElementType(firstObject, UIElementType::DROPDOWN);

	context.AcquireElement(firstObject);

	CHECK(fixture.mock->lastCreatedType == UIElementType::BUTTON);
}


TEST_CASE("The element is only built once, however many components share it")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	context.DeclareElementType(firstObject, UIElementType::BUTTON);

	auto first = context.AcquireElement(firstObject);
	auto second = context.AcquireElement(firstObject);

	CHECK(first == second);
	CHECK(fixture.mock->createElementCount == 1);
}


TEST_CASE("A deactivated component stops receiving UI events")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto active = MakeComponent(bucket);
	auto inactive = MakeComponent(bucket);

	fixture.ElementFor(firstObject).RegisterOwner(active);
	fixture.ElementFor(firstObject).RegisterOwner(inactive);

	// Everywhere else in the engine, deactivating a component stops it running. Events reach the UI
	// directly rather than through the scene traversal that enforces that, so it is enforced here.
	inactive->SetActive(false);

	Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id());

	CHECK(active->eventCount == 1);
	CHECK(inactive->eventCount == 0);
}


TEST_CASE("Reactivating a component makes it responsive again")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto component = MakeComponent(bucket);

	fixture.ElementFor(firstObject).RegisterOwner(component);

	component->SetActive(false);
	Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id());

	component->SetActive(true);
	Dispatch(*fixture.manager, fixture.ElementFor(firstObject).Id());

	CHECK(component->eventCount == 1);
}


TEST_CASE("A released object leaves nothing behind for a later object to inherit")
{
	auto fixture = Fixture();
	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto original = scene->Create<TestGameObject>();

	context.DeclareElementType(original, UIElementType::BUTTON);

	auto originalElement = context.AcquireElement(original);

	context.ReleaseElement(original);

	// A different object, which under an address-keyed map could land on the freed slot and be handed
	// the element that has just been destroyed. Handles carry a generation, so it gets its own.
	auto replacement = scene->Create<TestGameObject>();

	auto replacementElement = context.AcquireElement(replacement);

	CHECK(replacementElement != originalElement);
	CHECK(fixture.mock->createElementCount == 2);
}


TEST_CASE("Releasing through a handle works even once the object is gone")
{
	auto fixture = Fixture();
	auto context = fixture.manager->CreateCanvasContext(Vector2I(800, 600));

	auto scene = std::make_shared<FakeSceneHandleProvider>();
	auto owner = scene->Create<TestGameObject>();

	context.AcquireElement(owner);

	owner->Destroy();

	// The handle is still a usable key after the object it names has been destroyed, which is the whole
	// reason the map is not keyed by a raw pointer: this is exactly when a component's destructor runs.
	CHECK_NOTHROW(context.ReleaseElement(owner));
	CHECK(fixture.mock->destroyElementCount == 1);
}


TEST_SUITE_END();