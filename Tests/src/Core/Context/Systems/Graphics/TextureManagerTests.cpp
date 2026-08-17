#include <doctest.h>
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"


using namespace DF2D::Core;


TEST_SUITE_BEGIN("TextureManager");


TEST_CASE("Constructor succeeds with valid backend")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	CHECK(mockPtr->loadCount == 0);
}


TEST_CASE("LoadTexture delegates to backend with correct filename")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto tex = manager->LoadTexture("test.png");

	CHECK(tex == 1);
	CHECK(mockPtr->loadCount == 1);
	CHECK(mockPtr->lastLoadedFile == "test.png");
}


TEST_CASE("LoadTexture returns 0 when backend fails")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	mockPtr->failNextLoad = true;

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto tex = manager->LoadTexture("missing.png");

	CHECK(tex == 0);
	CHECK(mockPtr->loadCount == 1);
}


TEST_CASE("Backend failure is not cached")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	mockPtr->failNextLoad = true;

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	manager->LoadTexture("test.png");
	CHECK(mockPtr->loadCount == 1);

	manager->LoadTexture("test.png");
	CHECK(mockPtr->loadCount == 2);
}


TEST_CASE("Same filename is cached and loaded once")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto tex1 = manager->LoadTexture("test.png");
	auto tex2 = manager->LoadTexture("test.png");

	CHECK(tex1 == tex2);
	CHECK(mockPtr->loadCount == 1);
}


TEST_CASE("GetTextureSize returns Zero for unknown id")
{
	auto mock = std::make_unique<MockTextureBackend>();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto size = manager->GetTextureSize(999);

	CHECK(size.x == 0);
	CHECK(size.y == 0);
}


TEST_CASE("Different filenames call backend for each")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	manager->LoadTexture("a.png");
	manager->LoadTexture("b.png");

	CHECK(mockPtr->loadCount == 2);
}


TEST_CASE("ICoreSystem lifecycle methods are safe no-ops")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	ICoreSystem* system = manager.get();

	system->BeginFrame();

	CHECK(mockPtr->loadCount == 0);

	system->PreUpdate(0.0f);
	system->EndUpdate(0.0f);
	system->EndDraw();
}


TEST_CASE("GetTextureSize delegates to backend")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	mockPtr->nextSize = Vector2I{128, 256};

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto tex = manager->LoadTexture("test.png");
	auto size = manager->GetTextureSize(tex);

	CHECK(size.x == 128);
	CHECK(size.y == 256);
}


TEST_CASE("ClearCache unloads from backend and forces reload")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto tex = manager->LoadTexture("test.png");
	CHECK(tex != 0);
	CHECK(mockPtr->loadCount == 1);

	manager->ClearCache();
	CHECK(mockPtr->unloadCount == 1);

	manager->LoadTexture("test.png");
	CHECK(mockPtr->loadCount == 2);
}


TEST_SUITE_END();