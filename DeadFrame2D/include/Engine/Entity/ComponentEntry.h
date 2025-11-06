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


		ComponentEntry();

		~ComponentEntry();

		ComponentEntry(const ComponentEntry&) = delete;

		ComponentEntry& operator=(const ComponentEntry&) = delete;

		ComponentEntry(ComponentEntry&&) noexcept = default;

		ComponentEntry& operator=(ComponentEntry&&) noexcept = default;
	};
}