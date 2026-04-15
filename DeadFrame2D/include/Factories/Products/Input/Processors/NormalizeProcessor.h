#pragma once
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"


namespace DeadFrame2D::Factories
{
	class DF2D_API NormalizeProcessor final : public IInputProcessor
	{
	public:
		NormalizeProcessor() = default;


		Models::ProcessorType GetType() const override;

		void Process(float& value) const override;

		void Process(Core::Vector2F& value) const override;
	};
}