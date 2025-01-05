#include "Map/GameMap.h"
#include "Map/GameMapParser.h"
#include <sstream>
#include <tinyxml2.h>
#include <filesystem>


using namespace tinyxml2;


TileSet GameMapParser::ParseTileSet(XMLElement* xmlTileset)
{
	auto name = xmlTileset->Attribute("name");

	auto firstID = xmlTileset->IntAttribute("firstgid");

	auto tileCount = xmlTileset->IntAttribute("tilecount");
	
	// Allow using more than 1 tileset
	auto lastID = (firstID + tileCount) - 1;

	auto culumnCount = xmlTileset->IntAttribute("columns");
	
	auto rowCount = tileCount / culumnCount;

	auto tileSize = xmlTileset->IntAttribute("tilewidth");

	std::filesystem::path source = xmlTileset->FirstChildElement()->Attribute("source");
	
	auto fullPath = "Assets/Sprites/" + source.filename().string();

	return TileSet(firstID, lastID, rowCount, culumnCount, tileCount, tileSize, name, fullPath);
}

std::shared_ptr<TileLayer> GameMapParser::ParseTileLayer(XMLElement* xmlLayer, TileSetList tilesets, int tilesize, int rowcount, int colcount)
{
	// Get data (actual map)
	XMLElement* data{ nullptr };
	for (auto i = xmlLayer->FirstChildElement(); i != nullptr; i = i->NextSiblingElement())
	{
		// Enforce string comparison
		if (i->Value() == std::string("data"))
		{
			data = i;
			break;
		}
	}

	// Parse through tile map
	std::string matrix(data->GetText());
	std::istringstream iss(matrix);
	std::string id;

	TileMap tilemap(rowcount, std::vector<int>(colcount, 0));
	for (auto row = 0; row < rowcount; row++)
	{
		for (auto col = 0; col < colcount; col++)
		{
			// Discards ','
			std::getline(iss, id, ','); 

			std::stringstream convertor(id);
			convertor >> tilemap[row][col];

			if (!iss.good())
				break;
		}
	}
	
	iss.clear();

	return std::shared_ptr<TileLayer>(new TileLayer(tilesize, rowcount, colcount, tilemap, tilesets));
}

std::vector<std::shared_ptr<TileLayer>> GameMapParser::Parse(std::string source)
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

	// Parse tilesets
	TileSetList tilesets;
	for (auto i = root->FirstChildElement(); i != nullptr; i = i->NextSiblingElement())
	{
		// Enforce string comparison
		if (i->Value() == std::string("tileset"))
		{
			tilesets.push_back(ParseTileSet(i));
		}
	}

	// Parse Layers
	std::vector<std::shared_ptr<TileLayer>> layers = {};
	for (auto i = root->FirstChildElement(); i != nullptr; i = i->NextSiblingElement())
	{
		// Enforce string comparison
		if (i->Value() == std::string("layer"))
		{
			layers.push_back(ParseTileLayer(i, tilesets, tileSize, rowCount, colCount));
		}
	}

	return layers;
}