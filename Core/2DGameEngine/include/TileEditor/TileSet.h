#pragma once
#include <iostream>
#include <string>


class SDL_Texture;


struct TileSet
{
	std::shared_ptr<SDL_Texture> tileSetTexture;

	std::string name;

	std::string source;
	
	int firstID; 
	
	int lastID;
	
	int rowCount; 
	
	int culumnCount;
	
	int tileCount;
	
	int tileSize;


	TileSet(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& name, const std::string& source);
	

	bool IsValid() const;


	bool operator==(const TileSet& other) const;

	bool operator!=(const TileSet& other) const;

	friend std::ostream& operator<<(std::ostream& os, const TileSet& tileSet);
};