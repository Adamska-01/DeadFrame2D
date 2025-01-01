#pragma once
#include <array>
#include <stdexcept>
#include <Constants/BobbleConstants.h>


class Bobble;


struct BobbleNeighbors
{
	std::array<Bobble*, BobbleConstants::MAX_BOBBLE_NEIGHBOURS> neighborList;


	BobbleNeighbors();

	BobbleNeighbors(std::initializer_list<Bobble*> init);
};