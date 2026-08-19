#include "Utilities/Debugging/Guards.h"
#include "Utilities/Parsers/TileEditors/Tiled/TiledMapParser.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <tinyxml2.h>


namespace DF2D::Utilities
{
	using namespace tinyxml2;

	using namespace DF2D::Core;
	using namespace DF2D::Models;
	using namespace DF2D::Utilities;


	TiledMapParser::TiledMapParser(Core::TextureManager* tm)
		: textureManager(tm)
	{
	}

	void TiledMapParser::SetTextureManager(Core::TextureManager* tm)
	{
		textureManager = Guard::AgainstNullAssignment(tm, NAME_OF(tm));
	}

	TiledTileSet TiledMapParser::ParseTileSet(XMLElement* xmlTileset)
	{
		auto firstId = xmlTileset->IntAttribute("firstgid");
		auto columns = xmlTileset->IntAttribute("columns");
		auto tileCount = xmlTileset->IntAttribute("tilecount");

		// `columns` missing/zero would otherwise divide-by-zero; treat it as "no rows known".
		auto rowCount = columns > 0 ? tileCount / columns : 0;

		auto source = std::string();
		auto* imageElement = xmlTileset->FirstChildElement();

		if (imageElement != nullptr)
		{
			auto* sourceAttr = imageElement->Attribute("source");

			if (sourceAttr != nullptr)
			{
				source = "App/Assets/Sprites/" + std::filesystem::path(sourceAttr).filename().string();
			}
		}

		return TiledTileSet(
			firstId,
			firstId + tileCount - 1,
			rowCount,
			columns,
			tileCount,
			xmlTileset->IntAttribute("tilewidth"),
			source,
			textureManager);
	}

	TiledLayer TiledMapParser::ParseLayers(tinyxml2::XMLElement* xmlLayer, int rowCount, int colCount)
	{
		TiledLayer layer;

		auto* nameAttr = xmlLayer->Attribute("name");
		layer.name = nameAttr != nullptr ? nameAttr : "";

		// Parse the tile map data
		auto data = xmlLayer->FirstChildElement("data");
		layer.Data.resize(rowCount, std::vector<int>(colCount, 0));
		if (data != nullptr)
		{
			auto* text = data->GetText();

			if (text != nullptr)
			{
				std::istringstream iss(text);
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
		}

		// Parse the properties associated with the current layer;
		auto properties = xmlLayer->FirstChildElement("properties");
		if (properties != nullptr)
		{
			for (auto elem = properties->FirstChildElement("property"); elem; elem = elem->NextSiblingElement("property"))
			{
				TiledProperty property;

				auto* propNameAttr = elem->Attribute("name");
				auto* propTypeAttr = elem->Attribute("type");

				property.Name = propNameAttr != nullptr ? propNameAttr : "";
				property.Type = propTypeAttr != nullptr ? propTypeAttr : "";

				auto value = elem->Attribute("value");

				if (value != nullptr)
				{
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
				}

				layer.Properties[property.Name] = property;
			}
		}

		return layer;
	}

	TiledObjectGroup TiledMapParser::ParseObjectGroup(tinyxml2::XMLElement* xmlObjectGroup)
	{
		TiledObjectGroup group;

		auto* nameAttr = xmlObjectGroup->Attribute("name");
		group.name = nameAttr != nullptr ? nameAttr : "";

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

	std::shared_ptr<TiledMap> TiledMapParser::Parse(std::string_view source)
	{
		XMLDocument xml;
	
		if (xml.LoadFile(std::string(source).c_str()) != XMLError::XML_SUCCESS)
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
}