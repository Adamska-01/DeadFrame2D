#include "Core/Context/Systems/UI/UIManager.h"
#include "Engine/ECS/Entity/Component/Core/UI/UIComponent.h"
#include "Engine/ECS/Entity/Component/Storage/ComponentBucket.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Mocks/Context/Systems/UI/MockUIBackend.h"
#include <doctest.h>
#include <memory>


using namespace DF2D::Core;
using namespace DF2D::Data;


namespace
{
	struct Fixture
	{
		MockUIBackend* mock = nullptr;

		std::unique_ptr<UIManager> manager;


		/** @brief Fonts already registered by the constructor, so tests can measure their own deltas. */
		size_t baselineFontCount = 0;


		Fixture()
		{
			auto owned = std::make_unique<MockUIBackend>();

			mock = owned.get();
			manager = std::make_unique<UIManager>(std::move(owned));

			baselineFontCount = mock->loadedFonts.size();
		}
	};


	// Stand-ins for GameObjects: AcquireElement only ever uses the address as an identity.
	int firstObject = 0;
	int secondObject = 0;

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


TEST_CASE("CreateContext returns 0 and tracks nothing when the backend fails")
{
	auto fixture = Fixture();

	fixture.mock->failNextContext = true;

	CHECK(fixture.manager->CreateContext(Vector2I(800, 600)) == 0);
}


TEST_CASE("Live contexts are updated once per frame")
{
	auto fixture = Fixture();

	fixture.manager->CreateContext(Vector2I(800, 600));
	fixture.manager->CreateContext(Vector2I(800, 600));

	static_cast<ICoreSystem*>(fixture.manager.get())->EndUpdate(0.016f);

	CHECK(fixture.mock->updateContextCount == 2);
}


TEST_CASE("A destroyed context stops being updated")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateContext(Vector2I(800, 600));

	fixture.manager->DestroyContext(context);

	static_cast<ICoreSystem*>(fixture.manager.get())->EndUpdate(0.016f);

	CHECK(fixture.mock->destroyContextCount == 1);
	CHECK(fixture.mock->updateContextCount == 0);
}


TEST_CASE("Two UI components on one object share a single element")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateContext(Vector2I(800, 600));

	auto first = fixture.manager->AcquireElement(context, &firstObject, UIElementType::PANEL);
	auto second = fixture.manager->AcquireElement(context, &firstObject, UIElementType::PANEL);

	CHECK(first == second);
	CHECK(fixture.mock->createElementCount == 1);
}


TEST_CASE("Different objects get different elements")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateContext(Vector2I(800, 600));

	auto first = fixture.manager->AcquireElement(context, &firstObject, UIElementType::PANEL);
	auto second = fixture.manager->AcquireElement(context, &secondObject, UIElementType::PANEL);

	CHECK(first != second);
	CHECK(fixture.mock->createElementCount == 2);
}


TEST_CASE("A shared element survives until its last holder releases it")
{
	auto fixture = Fixture();

	auto context = fixture.manager->CreateContext(Vector2I(800, 600));

	fixture.manager->AcquireElement(context, &firstObject, UIElementType::PANEL);
	fixture.manager->AcquireElement(context, &firstObject, UIElementType::PANEL);

	fixture.manager->ReleaseElement(&firstObject);

	CHECK(fixture.mock->destroyElementCount == 0);

	fixture.manager->ReleaseElement(&firstObject);

	CHECK(fixture.mock->destroyElementCount == 1);
}


TEST_CASE("Releasing an object that never acquired anything is a no-op")
{
	auto fixture = Fixture();

	fixture.manager->ReleaseElement(&firstObject);

	CHECK(fixture.mock->destroyElementCount == 0);
}


TEST_CASE("AcquireElement refuses an invalid context")
{
	auto fixture = Fixture();

	CHECK(fixture.manager->AcquireElement(0, &firstObject, UIElementType::PANEL) == 0);
	CHECK(fixture.mock->createElementCount == 0);
}


TEST_CASE("RenderContext refuses the invalid context without reaching the backend")
{
	auto fixture = Fixture();

	auto drawList = fixture.manager->RenderContext(0);

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
	fixture.manager->RegisterElementOwner(42, first);
	fixture.manager->RegisterElementOwner(42, second);

	Dispatch(*fixture.manager, 42);

	CHECK(first->eventCount == 1);
	CHECK(second->eventCount == 1);
}


TEST_CASE("A destroyed component is skipped while its co-owners still receive events")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto first = MakeComponent(bucket);
	auto second = MakeComponent(bucket);

	fixture.manager->RegisterElementOwner(42, first);
	fixture.manager->RegisterElementOwner(42, second);

	// Destroyed without unregistering, which is what a destructor path that skips cleanup leaves
	// behind. The handle expires, so dispatch must drop it rather than dereference it.
	bucket->RemoveComponent(first);

	CHECK_NOTHROW(Dispatch(*fixture.manager, 42));
	CHECK(second->eventCount == 1);
}


TEST_CASE("Unregistering one component leaves the others subscribed")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto first = MakeComponent(bucket);
	auto second = MakeComponent(bucket);

	fixture.manager->RegisterElementOwner(42, first);
	fixture.manager->RegisterElementOwner(42, second);

	fixture.manager->UnregisterElementOwner(42, first);

	Dispatch(*fixture.manager, 42);

	CHECK(first->eventCount == 0);
	CHECK(second->eventCount == 1);
}


TEST_CASE("An element with no remaining owners stops dispatching")
{
	auto fixture = Fixture();
	std::shared_ptr<DF2D::Engine::ComponentBucket> bucket;

	auto only = MakeComponent(bucket);

	fixture.manager->RegisterElementOwner(42, only);
	fixture.manager->UnregisterElementOwner(42, only);

	CHECK_NOTHROW(Dispatch(*fixture.manager, 42));
	CHECK(only->eventCount == 0);
}


TEST_SUITE_END();