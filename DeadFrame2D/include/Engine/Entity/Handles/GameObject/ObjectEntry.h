#pragma once
#include <memory>


namespace DeadFrame2D::Engine
{
	class GameObject;


	struct ObjectEntry
	{
		std::unique_ptr<GameObject> object;

		uint32_t generation = 0;

		bool alive = false;


		ObjectEntry();

		~ObjectEntry();

		ObjectEntry(const ObjectEntry&) = delete;

		ObjectEntry& operator=(const ObjectEntry&) = delete;

		ObjectEntry(ObjectEntry&&) noexcept = default;

		ObjectEntry& operator=(ObjectEntry&&) noexcept = default;
	};
}