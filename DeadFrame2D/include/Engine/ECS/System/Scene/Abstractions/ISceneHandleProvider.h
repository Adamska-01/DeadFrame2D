#pragma once
#include "DF2D_API.h"
#include <cstdint>


namespace DF2D::Engine
{
	class GameObject;


	class DF2D_API ISceneHandleProvider
	{
	public:
		virtual ~ISceneHandleProvider() = default;


		virtual GameObject* GetAt(uint32_t index) const = 0;

		virtual bool IsValid(uint32_t index, uint32_t generation) const = 0;
	};
}