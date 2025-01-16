#pragma once
#include "IObject.h"


template <typename TMap>
class TileMapRenderer : public IObject 
{
protected:
	TMap tileMap;


public:
	virtual void Update(float dt) = 0;
	
	virtual void Draw() = 0;
	
	virtual void Clean() = 0;
};