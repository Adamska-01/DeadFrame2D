#pragma once
#include "Engine/ECS/Entity/Object/Handle/ObjectHandleBase.h"
#include <cstddef>


namespace DF2D::Utilities
{
	/**
	 * @brief Hashes an object handle so it can key a map.
	 *
	 * Only the slot and its generation are mixed in. That is deliberate: the generation is what makes a
	 * handle safe as a key, because a recycled slot produces a different handle rather than silently
	 * matching whatever used to live there. Handles from different scenes can collide in the hash and
	 * are separated by equality, which compares the scene too.
	 */
	struct ObjectHandleHash
	{
		std::size_t operator()(const Engine::ObjectHandleBase& handle) const noexcept
		{
			auto index = static_cast<std::size_t>(handle.GetIndex());
			auto generation = static_cast<std::size_t>(handle.GetGeneration());

			return index ^ (generation << 1);
		}
	};
}