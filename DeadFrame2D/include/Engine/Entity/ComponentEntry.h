#pragma once
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameComponent;


	struct ComponentEntry
	{
		std::unique_ptr<GameComponent> instance;

		uint32_t generation = 0;

		bool alive = false;
	};
}