#include "Core/SubSystems/Systems/TextureManager.h"
#include "Models/TileEditors/Tiled/TiledTileSet.h"
#include <ostream>
#include <string>


namespace DeadFrame2D::Models
{
	using namespace DeadFrame2D::Core;


	TiledTileSet::TiledTileSet(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& source)
		: firstID(firstID),
		lastID(lastID),
		rowCount(rowCount),
		columnCount(columnCount),
		tileCount(tileCount),
		tileSize(tileSize),
		source(source)
	{
		// Throw ?
		if (!IsValid())
			return;

		tileSetTexture = TextureManager::LoadTexture(source);
	}

	bool TiledTileSet::IsValid() const
	{
		return tileSize > 0 && rowCount >= 0 && columnCount >= 0 && !source.empty();
	}

	std::ostream& operator<<(std::ostream& os, const TiledTileSet& tileSet)
	{
		os << "Source: " << tileSet.source
			<< "\nFirstID: " << tileSet.firstID << ", LastID: " << tileSet.lastID
			<< "\nRows: " << tileSet.rowCount << ", Columns: " << tileSet.columnCount
			<< "\nTileCount: " << tileSet.tileCount << ", TileSize: " << tileSet.tileSize;

		return os;
	}
}