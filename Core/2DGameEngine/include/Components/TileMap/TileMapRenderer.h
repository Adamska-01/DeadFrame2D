#pragma once
#include "Components/GameComponent.h"


template <typename TMap>
class TileMapRenderer : public GameComponent
{
protected:
	TMap tileMap;


public:
	virtual void Init() = 0;

	virtual void Update(float dt) = 0;
	
	virtual void Draw() = 0;
	
	virtual void Clean() = 0;
};