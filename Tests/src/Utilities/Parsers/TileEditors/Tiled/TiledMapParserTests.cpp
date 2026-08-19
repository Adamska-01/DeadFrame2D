#include "Helpers/TempDirectory.h"
#include "Utilities/Parsers/TileEditors/Tiled/TiledMapParser.h"
#include <doctest.h>


using namespace DF2D::Models;
using namespace DF2D::Utilities;


TEST_SUITE_BEGIN("TiledMapParser");


TEST_CASE("Parse reads a well-formed .tmx into the TiledMap model")
{
	TempDirectory dir;
	auto path = dir.WriteFile("map.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map version="1.10" tiledversion="1.11.0" orientation="orthogonal" renderorder="right-down" width="10" height="5" tilewidth="16" tileheight="16">
	<tileset firstgid="1" name="ExampleTiles" tilewidth="16" tileheight="16" tilecount="10" columns="5">
		<image source="../Sprites/Example.png" width="80" height="32"/>
	</tileset>
	<layer id="1" name="ExampleLayer" width="10" height="5">
		<data encoding="csv">
		0,0,1,2,0,0,4,3,0,0,
		0,0,1,2,0,0,4,3,0,0,
		0,0,1,2,0,0,4,3,0,0,
		0,0,1,2,0,0,4,3,0,0,
		0,0,1,2,0,0,4,3,0,0
		</data>
	</layer>
	<objectgroup id="5" name="Player_Position">
		<object id="6" x="640" y="672">
			<point/>
		</object>
	</objectgroup>
</map>
)").string();

	auto parser = TiledMapParser();
	auto map = parser.Parse(path);

	REQUIRE(map != nullptr);
	CHECK(map->width == 10);
	CHECK(map->height == 5);
	CHECK(map->tileSize == 16);

	REQUIRE(map->tileSets.size() == 1);
	CHECK(map->tileSets[0].firstID == 1);
	CHECK(map->tileSets[0].lastID == 10);
	CHECK(map->tileSets[0].rowCount == 2);
	CHECK(map->tileSets[0].columnCount == 5);
	CHECK(map->tileSets[0].tileCount == 10);
	CHECK(map->tileSets[0].source == "App/Assets/Sprites/Example.png");

	REQUIRE(map->layers.size() == 1);
	CHECK(map->layers[0].name == "ExampleLayer");
	REQUIRE(map->layers[0].Data.size() == 5);
	REQUIRE(map->layers[0].Data[0].size() == 10);
	CHECK(map->layers[0].Data[0] == std::vector<int>{ 0, 0, 1, 2, 0, 0, 4, 3, 0, 0 });

	REQUIRE(map->objectGroups.size() == 1);
	CHECK(map->objectGroups[0].name == "Player_Position");
	REQUIRE(map->objectGroups[0].points.size() == 1);
	CHECK(map->objectGroups[0].points[0].x == doctest::Approx(640.0f));
	CHECK(map->objectGroups[0].points[0].y == doctest::Approx(672.0f));
}

TEST_CASE("Parse returns null for a missing file, without throwing")
{
	TempDirectory dir;
	auto missingPath = (dir.Path() / "does_not_exist.tmx").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(missingPath));
	CHECK(map == nullptr);
}


TEST_CASE("Parse does not crash (UB) on a tileset with no <image> child - it throws a catchable exception instead")
{
	// The missing <image>/source guard stops the old null-pointer UB, but TiledTileSet's own
	// IsValid() (pre-existing, unrelated to this fix) rejects an empty source as invalid tile
	// set data. So the safe outcome here is a normal, catchable exception, not a successful parse.
	TempDirectory dir;
	auto path = dir.WriteFile("no_image.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="1" height="1" tilewidth="16" tileheight="16">
	<tileset firstgid="1" tilecount="10" columns="5" tilewidth="16">
	</tileset>
</map>
)").string();

	auto parser = TiledMapParser();

	CHECK_THROWS_AS(parser.Parse(path), std::invalid_argument);
}

TEST_CASE("Parse does not crash on a tileset with columns=\"0\"")
{
	TempDirectory dir;
	auto path = dir.WriteFile("zero_columns.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="1" height="1" tilewidth="16" tileheight="16">
	<tileset firstgid="1" tilecount="10" columns="0" tilewidth="16">
		<image source="../Sprites/Example.png" width="80" height="32"/>
	</tileset>
</map>
)").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(path));
	REQUIRE(map != nullptr);
	REQUIRE(map->tileSets.size() == 1);
	CHECK(map->tileSets[0].columnCount == 0);
	CHECK(map->tileSets[0].rowCount == 0);
}

TEST_CASE("Parse does not crash on a layer with no name attribute")
{
	TempDirectory dir;
	auto path = dir.WriteFile("no_layer_name.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="2" height="1" tilewidth="16" tileheight="16">
	<layer width="2" height="1">
		<data encoding="csv">0,0</data>
	</layer>
</map>
)").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(path));
	REQUIRE(map != nullptr);
	REQUIRE(map->layers.size() == 1);
	CHECK(map->layers[0].name.empty());
}

TEST_CASE("Parse does not crash on a layer with an empty <data></data>")
{
	TempDirectory dir;
	auto path = dir.WriteFile("empty_data.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="2" height="1" tilewidth="16" tileheight="16">
	<layer name="EmptyData" width="2" height="1">
		<data encoding="csv"></data>
	</layer>
</map>
)").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(path));
	REQUIRE(map != nullptr);
	REQUIRE(map->layers.size() == 1);
	REQUIRE(map->layers[0].Data.size() == 1);
	REQUIRE(map->layers[0].Data[0].size() == 2);
	CHECK(map->layers[0].Data[0] == std::vector<int>{ 0, 0 });
}

TEST_CASE("Parse does not crash on a property with no name or type attribute")
{
	TempDirectory dir;
	auto path = dir.WriteFile("no_property_attrs.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="1" height="1" tilewidth="16" tileheight="16">
	<layer name="L" width="1" height="1">
		<data encoding="csv">0</data>
		<properties>
			<property value="123"/>
		</properties>
	</layer>
</map>
)").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(path));
	REQUIRE(map != nullptr);
	REQUIRE(map->layers.size() == 1);
	REQUIRE(map->layers[0].Properties.count("") == 1);
	CHECK(map->layers[0].Properties.at("").Name.empty());
	CHECK(map->layers[0].Properties.at("").Type.empty());
}

TEST_CASE("Parse does not crash on an object group with no name attribute")
{
	TempDirectory dir;
	auto path = dir.WriteFile("no_group_name.tmx", R"(<?xml version="1.0" encoding="UTF-8"?>
<map width="1" height="1" tilewidth="16" tileheight="16">
	<objectgroup>
		<object id="1" x="10" y="20">
			<point/>
		</object>
	</objectgroup>
</map>
)").string();

	auto parser = TiledMapParser();
	std::shared_ptr<TiledMap> map;

	CHECK_NOTHROW(map = parser.Parse(path));
	REQUIRE(map != nullptr);
	REQUIRE(map->objectGroups.size() == 1);
	CHECK(map->objectGroups[0].name.empty());
	REQUIRE(map->objectGroups[0].points.size() == 1);
	CHECK(map->objectGroups[0].points[0].x == doctest::Approx(10.0f));
	CHECK(map->objectGroups[0].points[0].y == doctest::Approx(20.0f));
}


TEST_SUITE_END();