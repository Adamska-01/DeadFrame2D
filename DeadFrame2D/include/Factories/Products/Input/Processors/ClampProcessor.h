#pragma once
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"


namespace DeadFrame2D::Factories
{
	class DF2D_API ClampProcessor final : public IInputProcessor
	{
	private:
		float min;

		float max;


	public:
		ClampProcessor(float min = 0.0f, float max = 1.0f);


		Shared::Models::ProcessorType GetType() const override;

		void Process(float& value) const override;

		void Process(DeadFrame2D::Core::Vector2F& value) const override;

		std::unique_ptr<IInputProcessor> Clone() const override;


		float GetMin() const;

		float GetMax() const;

		void SetMin(float min);

		void SetMax(float max);
	};
}