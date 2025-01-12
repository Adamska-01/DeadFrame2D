#pragma once
#include "TiledCompatibleLayer.h"
#include "TileEditor/Parsers/Tiled/Models/TiledMapWallProperties.h"
#include "TileEditor/Parsers/Tiled/Models/TiledSetModel.h"
#include "Tools/XML_Parser.h"
#include <memory>
#include <TileEditor/TileLayer.h>
#include <tinyxml2.h>


/*
Example XML map structure:

<?xml version="1.0" encoding="UTF-8"?>
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
</map>
*/


class TiledMapParser : public XML_Parser<std::vector<std::shared_ptr<TiledCompatibleLayer>>>
{
private:
	TiledSetModel ParseTileSet(tinyxml2::XMLElement* xmlTileset);

	std::vector<std::vector<int>> ParseTileMap(tinyxml2::XMLElement* xmlLayer, int rowcount, int colcount);

	std::optional<TiledMapWallProperties> ParseTileProperties(tinyxml2::XMLElement* xmlLayer);


public:
	virtual std::vector<std::shared_ptr<TiledCompatibleLayer>> Parse(std::string source) override;
};