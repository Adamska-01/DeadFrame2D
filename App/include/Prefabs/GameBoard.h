#pragma once
#include <GameObject.h>
#include <string_view>


class GameBoard : public GameObject
{
private:
	std::string_view mapSource;


public:
	GameBoard(std::string_view mapSource);


	virtual void ConstructGameObject() override;
};