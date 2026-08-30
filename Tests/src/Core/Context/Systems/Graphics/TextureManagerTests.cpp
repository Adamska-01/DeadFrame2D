#include <doctest.h>
#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include <cstdint>
#include <vector>


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


TEST_CASE("CreateTexture delegates raw pixels to the backend and records the size")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto pixels = std::vector<uint8_t>(4 * 2 * 4, 0xFF);

	auto tex = manager->CreateTexture(pixels, Vector2I(4, 2));

	CHECK(tex != 0);
	CHECK(mockPtr->createFromPixelsCount == 1);
	CHECK(mockPtr->lastCreatedSize == Vector2I(4, 2));
	CHECK(mockPtr->lastCreatedByteCount == pixels.size());
	CHECK(manager->GetTextureSize(tex) == Vector2I(4, 2));
}


TEST_CASE("CreateTexture returns 0 when the backend fails")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	mockPtr->failNextCreate = true;

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto pixels = std::vector<uint8_t>(4, 0);

	CHECK(manager->CreateTexture(pixels, Vector2I(1, 1)) == 0);
}


TEST_CASE("Generated textures are not entered into the path cache")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto pixels = std::vector<uint8_t>(4, 0);

	manager->CreateTexture(pixels, Vector2I(1, 1));
	manager->CreateTexture(pixels, Vector2I(1, 1));

	// Two creates from identical pixels must be two distinct textures; only file loads dedupe.
	CHECK(mockPtr->createFromPixelsCount == 2);
}


TEST_CASE("UnloadTexture releases a single texture and forgets its size")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto pixels = std::vector<uint8_t>(4, 0);
	auto tex = manager->CreateTexture(pixels, Vector2I(1, 1));

	manager->UnloadTexture(tex);

	CHECK(mockPtr->unloadCount == 1);
	CHECK(manager->GetTextureSize(tex) == Vector2I::Zero);
}


TEST_CASE("UnloadTexture ignores the invalid id")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	manager->UnloadTexture(0);

	CHECK(mockPtr->unloadCount == 0);
}


TEST_CASE("Unloading a file texture drops it from the path cache so it reloads")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();

	auto manager = std::make_unique<TextureManager>(std::move(mock));

	auto first = manager->LoadTexture("atlas.png");

	manager->UnloadTexture(first);

	// A stale cache entry here would hand back the destroyed id instead of loading again.
	auto second = manager->LoadTexture("atlas.png");

	CHECK(mockPtr->loadCount == 2);
	CHECK(second != first);
}


TEST_SUITE_END();