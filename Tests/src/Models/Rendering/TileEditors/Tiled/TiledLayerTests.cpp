#include <doctest.h>
#include "Models/Rendering/TileEditors/Tiled/TiledLayer.h"


using namespace DF2D::Models;


TEST_SUITE_BEGIN("TiledLayer");


TEST_CASE("GetFloatProperty returns stored value")
{
	TiledLayer layer;
	layer.Properties["speed"] = TiledProperty{ "speed", "float", 5.5f };

	CHECK(layer.GetFloatProperty("speed") == doctest::Approx(5.5f));
}


TEST_CASE("GetBoolProperty returns stored value")
{
	TiledLayer layer;
	layer.Properties["solid"] = TiledProperty{ "solid", "bool", true };

	CHECK(layer.GetBoolProperty("solid") == true);
}


TEST_CASE("GetIntProperty returns stored value")
{
	TiledLayer layer;
	layer.Properties["count"] = TiledProperty{ "count", "int", 7 };

	CHECK(layer.GetIntProperty("count") == 7);
}


TEST_CASE("GetStringProperty returns stored value")
{
	TiledLayer layer;
	layer.Properties["tag"] = TiledProperty{ "tag", "string", std::string("hazard") };

	CHECK(layer.GetStringProperty("tag") == "hazard");
}


TEST_CASE("Get*Property returns the supplied default when key is missing")
{
	TiledLayer layer;

	CHECK(layer.GetFloatProperty("missing", 1.5f) == doctest::Approx(1.5f));
	CHECK(layer.GetBoolProperty("missing", true) == true);
	CHECK(layer.GetIntProperty("missing", 42) == 42);
	CHECK(layer.GetStringProperty("missing", "fallback") == "fallback");
}


TEST_CASE("Get*Property returns the default when the stored type does not match")
{
	TiledLayer layer;
	layer.Properties["speed"] = TiledProperty{ "speed", "float", 5.5f };

	CHECK(layer.GetIntProperty("speed", -1) == -1);
	CHECK(layer.GetStringProperty("speed", "fallback") == "fallback");
}


TEST_SUITE_END();