#include "SubSystems/TextureManager.h"
#include "TileEditor/TileLayer.h"


TileLayer::TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TileSetList tileSets)
	: tileSize(tileSize),
	rowCount(rowCount),
	columnCount(colCount),
	tileMap(tileMap),
	tileSets(tileSets)
{
}

void TileLayer::Update(float dt)
{
}

void TileLayer::Draw()
{
	for (auto i = 0; i < rowCount; i++)
	{
		for (auto j = 0; j < columnCount; j++)
		{
			auto tileID = tileMap[i][j];

			if (tileID == 0)
				continue;

			auto index = 0;

			if (tileSets.size() > 1)
			{
				for (auto i = 0; i < tileSets.size(); i++)
				{
					if (tileID >= tileSets[i].firstID && tileID <= tileSets[i].lastID)
					{
						tileID = tileID + tileSets[i].tileCount - tileSets[i].lastID;
						index = i;
						break;
					}
				}
			}

			auto ts = tileSets[index];
			auto tileRow = tileID / ts.culumnCount;
			auto tileCol = tileID - tileRow * ts.culumnCount - 1;

			// if this tile is on the last column
			if (tileID % ts.culumnCount == 0)
			{
				tileRow--;
				tileCol = ts.culumnCount - 1;
			}

			auto src = SDL_Rect { ts.tileSize, ts.tileSize, tileRow * ts.tileSize, tileCol * ts.tileSize };
			auto dest = SDL_Rect{ j * ts.tileSize, i * ts.tileSize, ts.tileSize, ts.tileSize };

			TextureManager::DrawTile(ts.tileSetTexture.get(), src, dest);
		}
	}
}

void TileLayer::Clean()
{
}

TileMap TileLayer::GetTileMap()
{
	return tileMap;
}