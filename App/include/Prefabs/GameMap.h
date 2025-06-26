#pragma once
#include <string_view>
#include <GameObject.h>


class GameMap : public GameObject
{
private:
	std::string_view mapSource;


public:
	GameMap(std::string_view mapSource);


	virtual void ConstructGameObject() override;
};