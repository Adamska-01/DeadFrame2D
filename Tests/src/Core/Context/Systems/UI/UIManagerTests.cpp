#include "Core/Context/Systems/UI/UIManager.h"
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


TEST_SUITE_END();