#pragma once
#include "IObject.h"
#include <optional>
#include <vector>


template <typename TileMap, typename TileSetModel, typename Properties>
class TileLayer : public IObject 
{
protected:
	std::vector<std::vector<TileMap>> tileMap;
	
	std::vector<TileSetModel> tileSets;
	
	std::optional<Properties> properties;
	
	int tileSize;	
	
	int rowCount;
	
	int columnCount;


public:
	virtual void Update(float dt) = 0;
	
	virtual void Draw() = 0;
	
	virtual void Clean() = 0;


	std::vector<std::vector<TileMap>> GetTileMap();
};


template <typename TileMap, typename TileSetModel, typename Properties>
inline std::vector<std::vector<TileMap>> TileLayer<TileMap, TileSetModel, Properties>::GetTileMap()
{
	return tileMap;
}