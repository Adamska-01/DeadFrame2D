#pragma once
#include "DF2D_API.h"
#include <memory>
#include <SDL.h>
#include <string>


namespace DeadFrame2D::Models
{
	struct DF2D_API TiledTileSet
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
}