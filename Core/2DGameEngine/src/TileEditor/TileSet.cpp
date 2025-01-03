#include "SubSystems/TextureManager.h"
#include "TileEditor/TileSet.h"
#include <memory>


TileSet::TileSet(int firstID, int lastID, int rowCount, int columnCount, int tileCount, int tileSize, const std::string& name, const std::string& source)
	: firstID(firstID), 
	lastID(lastID), 
	rowCount(rowCount), 
	culumnCount(columnCount), 
	tileCount(tileCount), 
	tileSize(tileSize), 
	name(name), 
	source(source)
{
	if (!IsValid())
		return;

	tileSetTexture = TextureManager::LoadTexture(source);
}

bool TileSet::IsValid() const
{
	return tileSize > 0 && rowCount >= 0 && culumnCount >= 0 && !name.empty() && !source.empty();
}

bool TileSet::operator==(const TileSet& other) const
{
	return firstID == other.firstID && lastID == other.lastID &&
		rowCount == other.rowCount && culumnCount == other.culumnCount &&
		tileCount == other.tileCount && tileSize == other.tileSize &&
		name == other.name && source == other.source;
}

bool TileSet::operator!=(const TileSet& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const TileSet& tileSet)
{
	os	<< "TileSet: " << tileSet.name << ", Source: " << tileSet.source
		<< "\nFirstID: " << tileSet.firstID << ", LastID: " << tileSet.lastID
		<< "\nRows: " << tileSet.rowCount << ", Columns: " << tileSet.culumnCount
		<< "\nTileCount: " << tileSet.tileCount << ", TileSize: " << tileSet.tileSize;

	return os;
}