#include "TileEditor/Parsers/Tiled/Models/TiledMapWallProperties.h"
#include "TileEditor/Parsers/Tiled/Models/TiledSetModel.h"
#include <SubSystems/TextureManager.h>


TiledSetModel::TiledSetModel(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& name, const std::string& source)
	: firstID(firstID),
	lastID(lastID),
	rowCount(rowCount),
	columnCount(columnCount),
	tileCount(tileCount),
	tileSize(tileSize),
	name(name),
	source(source)
{
	// Throw ?
	if (!IsValid())
		return;

	tileSetTexture = TextureManager::LoadTexture(source);
}

bool TiledSetModel::IsValid() const
{
	return tileSize > 0 && rowCount >= 0 && columnCount >= 0 && !name.empty() && !source.empty();
}

std::ostream& operator<<(std::ostream& os, const TiledSetModel& tileSet)
{
	os << "TileSet: " << tileSet.name << ", Source: " << tileSet.source
		<< "\nFirstID: " << tileSet.firstID << ", LastID: " << tileSet.lastID
		<< "\nRows: " << tileSet.rowCount << ", Columns: " << tileSet.columnCount
		<< "\nTileCount: " << tileSet.tileCount << ", TileSize: " << tileSet.tileSize;

	return os;
}