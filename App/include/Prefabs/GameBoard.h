#pragma once
#include <GameObject.h>
#include <string_view>


class GameBoard : public GameObject
{
public:
	GameBoard(std::string_view mapSource);

	virtual ~GameBoard() override = default;
};