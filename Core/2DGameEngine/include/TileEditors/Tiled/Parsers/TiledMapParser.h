#pragma once
#include "TileEditors/Tiled/Models/TiledMap.h"
#include "Tools/XML_Parser.h"
#include <memory>
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


class TiledMapParser : public XML_Parser<std::shared_ptr<TiledMap>>
{
private:
	TiledTileSet ParseTileSet(tinyxml2::XMLElement* xmlTileset);

	TiledLayer ParseLayers(tinyxml2::XMLElement* xmlLayer, int rowcount, int colcount);


public:
	TiledMapParser() = default;
	
	~TiledMapParser() = default;


	virtual std::shared_ptr<TiledMap> Parse(std::string source) override;
};