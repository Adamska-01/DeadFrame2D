#pragma once
#include <memory>
#include <SDL.h>
#include <string>



struct TiledTileSet
{
	std::shared_ptr<SDL_Texture> tileSetTexture;

	std::string source;

	int firstID;

	int lastID;

	int rowCount;

	int columnCount;

	int tileCount;

	int tileSize;


	TiledTileSet(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& source);


	bool IsValid() const;


	friend std::ostream& operator<<(std::ostream& os, const TiledTileSet& tileSet);
};