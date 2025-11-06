#pragma once
#include "DF2D_API.h"
#include <cstdint>
#include <memory>


namespace DeadFrame2D::Engine
{
	class ISceneHandleProvider;


	class DF2D_API ObjectHandleBase
	{
		template<typename T>
		friend class ObjectHandle;


	protected:
		std::weak_ptr<ISceneHandleProvider> scene;

		uint32_t index = UINT32_MAX;

		uint32_t generation = 0;


		bool IsValid() const noexcept;


	public:
		ObjectHandleBase() = default;

		ObjectHandleBase(std::weak_ptr<ISceneHandleProvider> scene, uint32_t index, uint32_t generation);

		ObjectHandleBase(const ObjectHandleBase& other);


		bool operator==(const ObjectHandleBase& other) const noexcept;

		bool operator!=(const ObjectHandleBase& other) const noexcept;

		bool operator==(std::nullptr_t) const noexcept;

		bool operator!=(std::nullptr_t) const noexcept;

		explicit operator bool() const noexcept;

		ObjectHandleBase& operator=(std::nullptr_t);
	};
}
