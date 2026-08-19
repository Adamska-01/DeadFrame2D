#include "Helpers/TempDirectory.h"
#include "Utilities/IO/Serialization/JsonSerializer.h"
#include <doctest.h>
#include <nlohmann/json.hpp>


using namespace DF2D::Utilities;


namespace
{
	struct Point
	{
		int x = 0;

		int y = 0;


		bool operator==(const Point& other) const
		{
			return x == other.x && y == other.y;
		}
	};

	void to_json(nlohmann::json& j, const Point& p)
	{
		j = nlohmann::json{ { "x", p.x }, { "y", p.y } };
	}

	void from_json(const nlohmann::json& j, Point& p)
	{
		j.at("x").get_to(p.x);
		j.at("y").get_to(p.y);
	}

	// No to_json overload exists for this type, and it's not an STL container -
	// nlohmann::json(NotSerializable{}) fails to compile, which is exactly what
	// IsSerializable's SFINAE detects.
	struct NotSerializable
	{
		int value = 0;
	};
}


TEST_SUITE_BEGIN("JsonSerializer");


TEST_CASE("Serialize/Deserialize round-trips a value through nlohmann::json")
{
	auto original = Point{ .x = 3, .y = 4 };

	auto json = JsonSerializer::Serialize(original);
	auto result = JsonSerializer::Deserialize<Point>(json);

	CHECK(result == original);
}

TEST_CASE("SerializeToFile/DeserializeFromFile round-trips through disk")
{
	TempDirectory dir;
	auto filePath = (dir.Path() / "point.json").string();

	JsonSerializer::SerializeToFile(Point{ .x = 1, .y = 2 }, filePath);
	auto result = JsonSerializer::DeserializeFromFile<Point>(filePath);

	CHECK(result == Point{ .x = 1, .y = 2 });
}

TEST_CASE("DeserializeFromFile throws std::runtime_error for a missing file")
{
	TempDirectory dir;
	auto missingPath = (dir.Path() / "does_not_exist.json").string();

	CHECK_THROWS_AS(JsonSerializer::DeserializeFromFile<Point>(missingPath), std::runtime_error);
}

TEST_CASE("DeserializeFromFile rethrows on malformed JSON")
{
	TempDirectory dir;
	auto filePath = dir.WriteFile("malformed.json", "{ this is not valid json ").string();

	CHECK_THROWS_AS(JsonSerializer::DeserializeFromFile<Point>(filePath), nlohmann::json::parse_error);
}


TEST_CASE("IsObjectSerializable is true for a type with to_json/from_json")
{
	static_assert(JsonSerializer::IsObjectSerializable<Point>);
}

TEST_CASE("IsObjectSerializable is false for a type with no to_json overload")
{
	static_assert(!JsonSerializer::IsObjectSerializable<NotSerializable>);
}


TEST_SUITE_END();