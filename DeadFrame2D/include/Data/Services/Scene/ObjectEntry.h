#pragma once
#include "Data/Services/Scene/ObjectEntryState.h"
#include <memory>


namespace DF2D::Engine
{
	class GameObject;
}


namespace DF2D::Data
{
	struct ObjectEntry
	{
		std::unique_ptr<Engine::GameObject> object;

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