#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Mocks/Context/Systems/Graphics/MockTextureBackend.h"
#include "Models/Rendering/TileEditors/Tiled/TiledTileSet.h"
#include <doctest.h>
#include <sstream>


using namespace DF2D::Core;
using namespace DF2D::Models;


TEST_SUITE_BEGIN("TiledTileSet");


TEST_CASE("Valid tile set without a texture manager stays untextured")
{
	TiledTileSet tileSet(1, 10, 2, 5, 10, 32, "tiles.png", nullptr);

	CHECK(tileSet.IsValid());
	CHECK(tileSet.tileSetTexture == 0);
}


TEST_CASE("Valid tile set with a texture manager loads its texture")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();
	TextureManager textureManager(std::move(mock));

	TiledTileSet tileSet(1, 10, 2, 5, 10, 32, "tiles.png", &textureManager);

	CHECK(tileSet.IsValid());
	CHECK(tileSet.tileSetTexture != 0);
	CHECK(mockPtr->loadCount == 1);
	CHECK(mockPtr->lastLoadedFile == "tiles.png");
}


TEST_CASE("Non-positive tileSize throws instead of constructing an invalid tile set")
{
	CHECK_THROWS_AS(TiledTileSet(1, 10, 2, 5, 10, 0, "tiles.png", nullptr), std::invalid_argument);
}


TEST_CASE("Empty source throws instead of constructing an invalid tile set")
{
	CHECK_THROWS_AS(TiledTileSet(1, 10, 2, 5, 10, 32, "", nullptr), std::invalid_argument);
}


TEST_CASE("Invalid parameters never reach the texture backend")
{
	auto mock = std::make_unique<MockTextureBackend>();
	auto mockPtr = mock.get();
	TextureManager textureManager(std::move(mock));

	CHECK_THROWS_AS(TiledTileSet(1, 10, 2, 5, 10, 0, "tiles.png", &textureManager), std::invalid_argument);
	CHECK(mockPtr->loadCount == 0);
}


TEST_CASE("operator<< reports the tile set's fields")
{
	TiledTileSet tileSet(1, 10, 2, 5, 10, 32, "tiles.png", nullptr);

	std::ostringstream oss;
	oss << tileSet;

	CHECK(oss.str().find("tiles.png") != std::string::npos);
}


TEST_SUITE_END();