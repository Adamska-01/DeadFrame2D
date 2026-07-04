#include <doctest.h>
#include "Core/Context/Systems/UI/UIManager.h"
#include "Mocks/Context/Systems/UI/MockTextBackend.h"


using namespace DF2D::Core;
using namespace DF2D::Data;


static std::unique_ptr<UIManager> MakeManager(MockTextBackend*& outMock)
{
	auto mock = std::make_unique<MockTextBackend>();
	outMock = mock.get();

	return std::make_unique<UIManager>(std::move(mock));
}


TEST_SUITE_BEGIN("UIManager");


TEST_CASE("LoadFont delegates path and size to backend")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	auto font = manager->LoadFont("font.ttf", 24);

	CHECK(font == 1);
	CHECK(mock->loadFontCount == 1);
	CHECK(mock->lastFontPath == "font.ttf");
	CHECK(mock->lastFontSize == 24);
}


TEST_CASE("LoadFont caches by source and size")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	auto font1 = manager->LoadFont("font.ttf", 24);
	auto font2 = manager->LoadFont("font.ttf", 24);

	CHECK(font1 == font2);
	CHECK(mock->loadFontCount == 1);
}


TEST_CASE("Same source with different sizes produces separate cache entries")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	auto small = manager->LoadFont("font.ttf", 12);
	auto large = manager->LoadFont("font.ttf", 48);

	CHECK(small != large);
	CHECK(mock->loadFontCount == 2);
}


TEST_CASE("LoadFont failure returns 0 and is not cached")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	mock->failNextFontLoad = true;

	auto font1 = manager->LoadFont("font.ttf", 24);

	CHECK(font1 == 0);
	CHECK(mock->loadFontCount == 1);

	mock->failNextFontLoad = false;

	auto font2 = manager->LoadFont("font.ttf", 24);

	CHECK(font2 != 0); // Not cached — re-attempted
	CHECK(mock->loadFontCount == 2);
}


TEST_CASE("SetFontStyle resolves font and forwards style")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->SetFontStyle("font.ttf", 24, FontStyle::BOLD);

	CHECK(mock->setFontStyleCount == 1);
	CHECK(mock->lastStyledFont == 1);
	CHECK(mock->lastStyle == FontStyle::BOLD);
}


TEST_CASE("SetFontStyle with failing font does not call backend")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	mock->failNextFontLoad = true;

	manager->SetFontStyle("missing.ttf", 24, FontStyle::ITALIC);

	CHECK(mock->setFontStyleCount == 0);
}


TEST_CASE("LoadText with empty text returns zero-texture without touching backend")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	auto result = manager->LoadText("font.ttf", 24, "", {255, 255, 255, 255});

	CHECK(result.textureID == 0);
	CHECK(result.size.x == 0);
	CHECK(result.size.y == 0);
	CHECK(mock->loadFontCount == 0);
	CHECK(mock->createTextTextureCount == 0);
}


TEST_CASE("LoadText with failing font returns zero-texture and does not render")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	mock->failNextFontLoad = true;

	auto result = manager->LoadText("font.ttf", 24, "Hello", {255, 255, 255, 255});

	CHECK(result.textureID == 0);
	CHECK(result.size.x == 0);
	CHECK(result.size.y == 0);
	CHECK(mock->createTextTextureCount == 0);
}


TEST_CASE("LoadText renders single line and returns backend texture with size")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	mock->nextTextureId = 55;
	mock->nextTextureSize = { 320, 48 };

	auto result = manager->LoadText("font.ttf", 24, "Hello", {255, 0, 0, 255});

	CHECK(result.textureID == 55);
	CHECK(result.size.x == 320);
	CHECK(result.size.y == 48);
	CHECK(mock->createTextTextureCount == 1);
	CHECK(mock->lastTextFont == 1);

	REQUIRE(mock->lastLines.size() == 1);
	CHECK(mock->lastLines[0] == "Hello");
	CHECK(mock->lastColor == Color{255, 0, 0, 255});
}


TEST_CASE("LoadText splits text into lines")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->LoadText("font.ttf", 24, "first\nsecond", {});

	REQUIRE(mock->lastLines.size() == 2);
	CHECK(mock->lastLines[0] == "first");
	CHECK(mock->lastLines[1] == "second");
}


TEST_CASE("LoadText replaces empty lines with a space")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->LoadText("font.ttf", 24, "a\n\nb", {});

	REQUIRE(mock->lastLines.size() == 3);
	CHECK(mock->lastLines[0] == "a");
	CHECK(mock->lastLines[1] == " ");
	CHECK(mock->lastLines[2] == "b");
}


TEST_CASE("LoadText expands tabs to the next tab stop")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->LoadText("font.ttf", 24, "\tx\nab\tc", {});

	REQUIRE(mock->lastLines.size() == 2);
	CHECK(mock->lastLines[0] == "    x");  // Tab at column 0 -> 4 spaces
	CHECK(mock->lastLines[1] == "ab  c");  // Tab at column 2 -> 2 spaces
}


TEST_CASE("LoadText forwards centerText flag")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->LoadText("font.ttf", 24, "Hello", {}, true);
	CHECK(mock->lastCenterText == true);

	manager->LoadText("font.ttf", 24, "Hello", {}, false);
	CHECK(mock->lastCenterText == false);
}


TEST_CASE("LoadText reuses cached font across calls")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	manager->LoadText("font.ttf", 24, "one", {});
	manager->LoadText("font.ttf", 24, "two", {});

	CHECK(mock->loadFontCount == 1);
	CHECK(mock->createTextTextureCount == 2);
}


TEST_CASE("ICoreSystem lifecycle methods are safe no-ops")
{
	MockTextBackend* mock = nullptr;
	auto manager = MakeManager(mock);

	ICoreSystem* system = manager.get();

	system->BeginFrame();
	system->PreUpdate(0.0f);
	system->EndUpdate(0.0f);
	system->EndDraw();

	CHECK(mock->loadFontCount == 0);
	CHECK(mock->createTextTextureCount == 0);
}


TEST_SUITE_END();
