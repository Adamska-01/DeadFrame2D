#pragma once
#include "DF2D_API.h"
#include <cstdint>
#include <memory>


namespace DF2D::Engine
{
	class ComponentBucket;


	/**
	 * @brief Non-templated base class for all component handles.
	 * Stores shared handle data (bucket, index, generation) and allows
	 * GameComponent to reference itself safely without including templates.
	 */
	class DF2D_API ComponentHandleBase
	{
	protected:
		std::weak_ptr<ComponentBucket> bucket;

		uint32_t index = UINT32_MAX;

		uint32_t generation = 0;


	public:
		ComponentHandleBase() = default;

		ComponentHandleBase(std::weak_ptr<ComponentBucket> bucket, uint32_t index, uint32_t generation);

		ComponentHandleBase(const ComponentHandleBase& other);


		bool IsValid() const;


		bool operator==(const ComponentHandleBase& other) const;

		bool operator!=(const ComponentHandleBase& other) const;

		bool operator==(std::nullptr_t) const;

		bool operator!=(std::nullptr_t) const;

		explicit operator bool() const;

		ComponentHandleBase& operator=(std::nullptr_t);


		const ComponentBucket* GetBucket() const noexcept;

		uint32_t GetIndex() const noexcept;

		uint32_t GetGeneration() const noexcept;
	};
}