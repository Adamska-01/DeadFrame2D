#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Models/Input/ActionMap/Types/ProcessorType.h"


namespace DeadFrame2D::Factories
{
	class DF2D_API IInputProcessor
	{
	public:
		virtual ~IInputProcessor() = default;


		virtual Models::ProcessorType GetType() const = 0;

		virtual void Process(float& value) const = 0;

		virtual void Process(Core::Vector2F& value) const = 0;
	};
}