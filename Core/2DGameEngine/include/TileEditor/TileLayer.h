#pragma once
#include "IObject.h"
#include "SubSystems/Renderer.h"
#include "TileEditor/TileSet.h"
#include "SubSystems/TextureManager.h"
#include <memory>
#include <optional>
#include <vector>


using TileSetList = std::vector<TileSet>;

using TileMap = std::vector<std::vector<int>>;


template <typename T>
class TileLayer : public IObject 
{
private:
	TileMap tileMap;
	
	TileSetList tileSets;
	
	std::optional<T> properties;
	
	int tileSize;
	
	int rowCount;
	
	int columnCount;


public:
	TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TileSetList tileSets, std::optional<T> properties)
		: tileSize(tileSize),
		rowCount(rowCount),
		columnCount(colCount),
		tileMap(tileMap),
		tileSets(tileSets),
		properties(properties) 
	{
		Renderer::SetResolutionTarget({ columnCount * tileSize, rowCount * tileSize });
	}


	virtual void Update(float dt) override 
	{
		// Update logic here (currently empty)
	}

	virtual void Draw() override 
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
				auto tileRow = tileID / ts.culumnCount;
				auto tileCol = tileID - tileRow * ts.culumnCount - 1;

				// If this tile is on the last column
				if (tileID % ts.culumnCount == 0)
				{
					tileRow--;
					tileCol = ts.culumnCount - 1;
				}

				auto src = SDL_Rect{ tileCol * ts.tileSize, tileRow * ts.tileSize, ts.tileSize, ts.tileSize };
				auto dest = SDL_Rect{ j * ts.tileSize, i * ts.tileSize, ts.tileSize, ts.tileSize };

				TextureManager::DrawTile(ts.tileSetTexture.get(), src, dest);
			}
		}
	}

	virtual void Clean() override 
	{
	}

	TileMap GetTileMap() 
	{
		return tileMap;
	}
};