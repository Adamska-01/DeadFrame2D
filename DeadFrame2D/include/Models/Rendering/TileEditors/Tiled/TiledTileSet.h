#pragma once
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Models
{
	struct DF2D_API TiledTileSet
	{
		Data::TextureID tileSetTexture = 0;

		std::string source;

		int firstID;

		int lastID;

		int rowCount;

		int columnCount;

		int tileCount;

		int tileSize;


		TiledTileSet(
			int firstID,
			int lastID,
			int rowCount,
			int columnCount,
			int tileCount,
			int tileSize,
			const std::string& source,
			Core::TextureManager* tm);


		bool IsValid() const;


		friend DF2D_API std::ostream& operator<<(std::ostream& os, const TiledTileSet& tileSet);
	};
}