#pragma once
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"


namespace DF2D::Factories
{
	class DF2D_API ClampProcessor final : public IInputProcessor
	{
	private:
		float min;

		float max;


	public:
		ClampProcessor(float min = 0.0f, float max = 1.0f);


		Models::ProcessorType GetType() const override;

		void Process(float& value) const override;

		void Process(Core::Vector2F& value) const override;

		float GetMin() const;

		float GetMax() const;

		void SetMin(float min);

		void SetMax(float max);
	};
}