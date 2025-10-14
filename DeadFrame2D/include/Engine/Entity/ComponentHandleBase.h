#pragma once
#include "DF2D_API.h"
#include <cstdint>
#include <memory>


namespace DeadFrame2D::Engine
{
	class ComponentBucket;


	/**
	 * @brief Non-templated base class for all component handles.
	 * Stores shared handle data (bucket, index, generation) and allows
	 * GameComponent to reference itself safely without including templates.
	 */
	class DF2D_API ComponentHandleBase
	{
		template<typename T>
		friend class ComponentHandle;


	protected:
		std::weak_ptr<ComponentBucket> bucket;

		uint32_t index = UINT32_MAX;

		uint32_t generation = 0;


	public:
		ComponentHandleBase() = default;

		ComponentHandleBase(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation);

		ComponentHandleBase(const ComponentHandleBase& other);


		bool IsValid() const noexcept;
	};
}