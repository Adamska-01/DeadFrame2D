#include "TileEditor/Parsers/Tiled/TiledCompatibleLayer.h"
#include <SubSystems/Renderer.h>
#include <SubSystems/TextureManager.h>


TiledCompatibleLayer::TiledCompatibleLayer(int tileSize, int rowCount, int colCount, std::vector<std::vector<int>> tileMap, std::vector<TiledSetModel> tileSets, std::optional<TiledMapWallProperties> properties)
{
	this->tileSize = tileSize;
	this->rowCount = rowCount;
	this->columnCount = colCount;
	this->tileMap = tileMap;
	this->tileSets = tileSets;
	this->properties = properties;

	Renderer::SetResolutionTarget({ columnCount * tileSize, rowCount * tileSize });
}

void TiledCompatibleLayer::Update(float dt)
{
}

void TiledCompatibleLayer::Draw()
{
	for (auto i = 0; i < rowCount; i++)
	{
		for (auto j = 0; j < columnCount; j++)
		{
			auto tileID = tileMap[i][j];

			if (tileID == 0)
				continue;

			auto index = 0;

			// If there is more than 1 TileSet, normalize the tileID and get the TileSet index
			for (auto i = 1; i < tileSets.size(); i++)
			{
				if (tileID < tileSets[i].firstID || tileID > tileSets[i].lastID)
					continue;

				tileID = tileID + tileSets[i].tileCount - tileSets[i].lastID;
				index = i;

				break;
			}

			const auto& ts = tileSets[index];
			auto tileRow = tileID / ts.columnCount;
			auto tileCol = tileID - tileRow * ts.columnCount - 1;

			// If this tile is on the last column
			if (tileID % ts.columnCount == 0)
			{
				tileRow--;
				tileCol = ts.columnCount - 1;
			}

			auto src = SDL_Rect{ tileCol * ts.tileSize, tileRow * ts.tileSize, ts.tileSize, ts.tileSize };
			auto dest = SDL_Rect{ j * ts.tileSize, i * ts.tileSize, ts.tileSize, ts.tileSize };

			TextureManager::DrawTile(ts.tileSetTexture.get(), src, dest);
		}
	}
}

void TiledCompatibleLayer::Clean()
{
}
