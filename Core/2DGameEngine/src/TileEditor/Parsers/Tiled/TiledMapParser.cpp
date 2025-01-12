#include "TileEditor/Parsers/Tiled/TiledMapParser.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <tinyxml2.h>


using namespace tinyxml2;


TiledSetModel TiledMapParser::ParseTileSet(XMLElement* xmlTileset)
{
	return TiledSetModel(
		xmlTileset->IntAttribute("firstgid"),
		xmlTileset->IntAttribute("firstgid") + xmlTileset->IntAttribute("tilecount") - 1,
		xmlTileset->IntAttribute("tilecount") / xmlTileset->IntAttribute("columns"),
		xmlTileset->IntAttribute("columns"),
		xmlTileset->IntAttribute("tilecount"),
		xmlTileset->IntAttribute("tilewidth"),
		xmlTileset->Attribute("name"),
		"Assets/Sprites/" + std::filesystem::path(xmlTileset->FirstChildElement()->Attribute("source")).filename().string());
}

std::vector<std::vector<int>> TiledMapParser::ParseTileMap(tinyxml2::XMLElement* xmlLayer, int rowCount, int colCount)
{
	auto data = xmlLayer->FirstChildElement("data");
	auto properties = xmlLayer->FirstChildElement("properties");

	// Parse the tile map data
	std::vector<std::vector<int>> tileMap(rowCount, std::vector<int>(colCount, 0));
	if (data != nullptr)
	{
		std::istringstream iss(data->GetText());
		std::string id;

		for (auto row = 0; row < rowCount; ++row)
		{
			for (auto col = 0; col < colCount; ++col)
			{
				if (!std::getline(iss, id, ','))
					break;

				tileMap[row][col] = std::stoi(id);
			}
		}
	}

	return tileMap;
}

std::optional<TiledMapWallProperties> TiledMapParser::ParseTileProperties(tinyxml2::XMLElement* xmlLayer)
{
	auto properties = xmlLayer->FirstChildElement("properties");
	
	if (properties != nullptr && properties->FirstChildElement())
		return TiledMapWallProperties(properties->FirstChildElement()->FloatAttribute("value"));

	return std::nullopt;
}

std::vector<std::shared_ptr<TiledCompatibleLayer>> TiledMapParser::Parse(std::string source)
{
	XMLDocument xml;
	
	if (xml.LoadFile(source.c_str()) != XMLError::XML_SUCCESS)
	{
		std::cerr << "Failed to load: " << source << std::endl;

		return {};
	}

	auto root = xml.RootElement();
	
	auto colCount = root->IntAttribute("width", 0);
	auto rowCount = root->IntAttribute("height", 0);
	auto tileSize = root->IntAttribute("tilewidth", 0);

	// Parse tileSets
	std::vector<TiledSetModel> tileSets;
	for (auto elem = root->FirstChildElement("tileset"); elem; elem = elem->NextSiblingElement("tileset"))
	{
		tileSets.push_back(ParseTileSet(elem));
	}

	// Parse Layers
	std::vector<std::shared_ptr<TiledCompatibleLayer>> layers;
	for (auto elem = root->FirstChildElement("layer"); elem; elem = elem->NextSiblingElement("layer"))
	{
		layers.push_back(std::make_shared<TiledCompatibleLayer>(
			tileSize,
			rowCount,
			colCount,
			ParseTileMap(elem, rowCount, colCount),
			tileSets,
			ParseTileProperties(elem)));
	}

	return layers;
}