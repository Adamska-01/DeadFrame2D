#include "Generic/Bobble/BobbleNeighbors.h"


BobbleNeighbors::BobbleNeighbors()
	: neighborList{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
{
}

BobbleNeighbors::BobbleNeighbors(std::initializer_list<Bobble*> init)
{
	std::fill(neighborList.begin(), neighborList.end(), nullptr);

	std::copy(init.begin(), init.end(), neighborList.begin());
}