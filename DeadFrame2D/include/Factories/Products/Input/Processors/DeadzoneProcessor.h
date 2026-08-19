#pragma once
#include "Factories/Products/Input/Processors/Abstractions/IInputProcessor.h"


namespace DF2D::Factories
{
	class DF2D_API DeadzoneProcessor final : public IInputProcessor
	{
	private:
		static constexpr float MAX_THRESHOLD = 0.999f;


		float threshold;


	public:
		DeadzoneProcessor(float threshold = 0.15f);


		Models::ProcessorType GetType() const override;

		void Process(float& value) const override;

		void Process(Core::Vector2F& value) const override;

		float GetThreshold() const;

		void SetThreshold(float threshold);
	};
}