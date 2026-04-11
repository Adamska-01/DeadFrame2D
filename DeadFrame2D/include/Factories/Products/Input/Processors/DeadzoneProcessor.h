#pragma once
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"


namespace DeadFrame2D::Factories
{
	class DF2D_API DeadzoneProcessor final : public IInputProcessor
	{
	private:
		float threshold;


	public:
		DeadzoneProcessor(float threshold = 0.15f);


		Shared::Models::ProcessorType GetType() const override;

		void Process(float& value) const override;

		void Process(DeadFrame2D::Core::Vector2F& value) const override;

		float GetThreshold() const;

		void SetThreshold(float threshold);
	};
}