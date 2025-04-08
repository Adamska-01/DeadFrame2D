#include "TileEditors/Tiled/Parsers/TiledMapParser.h"
#include <filesystem>
#include <iostream>
#include <tinyxml2.h>


using namespace tinyxml2;


TiledTileSet TiledMapParser::ParseTileSet(XMLElement* xmlTileset)
{
	return TiledTileSet(
		xmlTileset->IntAttribute("firstgid"),
		xmlTileset->IntAttribute("firstgid") + xmlTileset->IntAttribute("tilecount") - 1,
		xmlTileset->IntAttribute("tilecount") / xmlTileset->IntAttribute("columns"),
		xmlTileset->IntAttribute("columns"),
		xmlTileset->IntAttribute("tilecount"),
		xmlTileset->IntAttribute("tilewidth"),
		"Assets/Sprites/" + std::filesystem::path(xmlTileset->FirstChildElement()->Attribute("source")).filename().string());
}

TiledLayer TiledMapParser::ParseLayers(tinyxml2::XMLElement* xmlLayer, int rowCount, int colCount)
{
	TiledLayer layer;

	// Parse the tile map data
	auto data = xmlLayer->FirstChildElement("data");
	layer.Data.resize(rowCount, std::vector<int>(colCount, 0));
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

				layer.Data[row][col] = std::stoi(id);
			}
		}
	}
	
	// Parse the properties associated with the current layer;
	auto properties = xmlLayer->FirstChildElement("properties");
	if (properties != nullptr)
	{
		for (auto elem = properties->FirstChildElement("property"); elem; elem = elem->NextSiblingElement("property"))
		{
			TiledProperty property;

			property.Name = elem->Attribute("name");
			property.Type = elem->Attribute("type");

			auto value = elem->Attribute("value");

			if (property.Type == "float") 
			{
				property.Value = std::stof(value);
			}
			else if (property.Type == "int") 
			{
				property.Value = std::stoi(value);
			}
			else if (property.Type == "bool") 
			{
				property.Value = (std::string(value) == "true");
			}
			else if (property.Type == "string") 
			{
				property.Value = std::string(value);
			}

			layer.Properties[property.Name] = property;
		}
	}

	return layer;
}

TiledObjectGroup TiledMapParser::ParseObjectGroup(tinyxml2::XMLElement* xmlObjectGroup)
{
	TiledObjectGroup group;

	group.name = xmlObjectGroup->Attribute("name");

	for (auto objElem = xmlObjectGroup->FirstChildElement("object"); objElem; objElem = objElem->NextSiblingElement("object"))
	{
		if (objElem->FirstChildElement("point"))
		{
			group.points.push_back(Vector2F
			{
				objElem->FloatAttribute("x"),
				objElem->FloatAttribute("y")
			});
		}
		// Could also add support for other objects
		//else if (objElem->FirstChildElement("ellipse"))
		//{
		//}
	}

	return group;
}

std::shared_ptr<TiledMap> TiledMapParser::Parse(std::string source)
{
	XMLDocument xml;
	
	if (xml.LoadFile(source.c_str()) != XMLError::XML_SUCCESS)
	{
		std::cerr << "Failed to load: " << source << std::endl;

		return {};
	}

	auto root = xml.RootElement();

	auto tileMap = std::make_shared<TiledMap>();
	
	tileMap->width = root->IntAttribute("width", 0);
	tileMap->height = root->IntAttribute("height", 0);
	tileMap->tileSize = root->IntAttribute("tilewidth", 0);

	// Parse tileSets
	std::vector<TiledTileSet> tileSets;
	for (auto elem = root->FirstChildElement("tileset"); elem; elem = elem->NextSiblingElement("tileset"))
	{
		tileMap->tileSets.push_back(ParseTileSet(elem));
	}

	// Parse layers
	std::vector<std::shared_ptr<TiledLayer>> layers;
	for (auto elem = root->FirstChildElement("layer"); elem; elem = elem->NextSiblingElement("layer"))
	{
		tileMap->layers.push_back(ParseLayers(elem, tileMap->height, tileMap->width));
	}

	// Parse object groups
	for (auto elem = root->FirstChildElement("objectgroup"); elem; elem = elem->NextSiblingElement("objectgroup"))
	{
		tileMap->objectGroups.push_back(ParseObjectGroup(elem));
	}

	return tileMap;
}