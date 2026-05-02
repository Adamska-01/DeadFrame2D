#pragma once
#include "Data/Services/Scene/ObjectEntryState.h"
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;
}


namespace DeadFrame2D::Data
{
	struct ObjectEntry
	{
		std::unique_ptr<DeadFrame2D::Engine::GameObject> object;

		uint32_t generation = 0;

		ObjectEntryState state = ObjectEntryState::DEAD;


		ObjectEntry();

		~ObjectEntry();

		ObjectEntry(const ObjectEntry&) = delete;

		ObjectEntry& operator=(const ObjectEntry&) = delete;

		ObjectEntry(ObjectEntry&&) noexcept = default;

		ObjectEntry& operator=(ObjectEntry&&) noexcept = default;
	};
}