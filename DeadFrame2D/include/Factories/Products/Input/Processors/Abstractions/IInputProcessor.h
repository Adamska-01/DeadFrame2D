#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include <memory>
#include <Models/Input/ActionMap/Types/ProcessorType.h>
#include <variant>
#include <vector>


namespace DeadFrame2D::Factories
{
	class DF2D_API IInputProcessor
	{
	public:
		virtual ~IInputProcessor() = default;


		virtual Shared::Models::ProcessorType GetType() const = 0;

		virtual void Process(float& value) const = 0;

		virtual void Process(DeadFrame2D::Core::Vector2F& value) const = 0;
	};
}