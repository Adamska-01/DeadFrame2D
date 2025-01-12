#pragma once
#include <memory>
#include <SDL.h>
#include <string>


struct TiledSetModel 
{
	std::shared_ptr<SDL_Texture> tileSetTexture;

	std::string name;

	std::string source;

	int firstID;

	int lastID;

	int rowCount;

	int columnCount;

	int tileCount;

	int tileSize;


	TiledSetModel(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& name, const std::string& source);


	bool IsValid() const;


	friend std::ostream& operator<<(std::ostream& os, const TiledSetModel& tileSet);
};