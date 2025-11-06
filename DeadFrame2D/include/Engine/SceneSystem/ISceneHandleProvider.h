#pragma once
#include "DF2D_API.h"
#include <cstdint>


namespace DeadFrame2D::Engine
{
	class GameObject;


	class DF2D_API ISceneHandleProvider
	{
		friend class ObjectHandleBase;

		template<typename T>
		friend class ObjectHandle;


	public:
		virtual ~ISceneHandleProvider() = default;


	protected:
		virtual GameObject* GetAt(uint32_t index) const = 0;

		virtual bool IsValid(uint32_t index, uint32_t generation) const = 0;
	};
}