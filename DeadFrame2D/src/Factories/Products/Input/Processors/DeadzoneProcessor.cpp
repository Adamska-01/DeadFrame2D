#include "Factories/Products/Input/Processors/DeadzoneProcessor.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Models;


	DeadzoneProcessor::DeadzoneProcessor(const float threshold)
		: threshold(std::clamp(threshold, 0.0f, MAX_THRESHOLD))
	{
	}

	ProcessorType DeadzoneProcessor::GetType() const
	{
		return ProcessorType::DEADZONE;
	}

	void DeadzoneProcessor::Process(float& value) const
	{
		if (std::abs(value) < threshold)
		{
			value = 0.0f;

			return;
		}

		const auto sign = (value > 0.0f) ? 1.0f : -1.0f;

		value = (value - sign * threshold) / (1.0f - threshold);
	}

	void DeadzoneProcessor::Process(Vector2F& value) const
	{
		if (std::abs(value.x) < threshold)
		{
			value.x = 0.0f;
		}
		else
		{
			const auto sign = (value.x > 0.0f) ? 1.0f : -1.0f;

			value.x = (value.x - sign * threshold) / (1.0f - threshold);
		}

		if (std::abs(value.y) < threshold)
		{
			value.y = 0.0f;
		}
		else
		{
			const auto sign = (value.y > 0.0f) ? 1.0f : -1.0f;

			value.y = (value.y - sign * threshold) / (1.0f - threshold);
		}
	}

	float DeadzoneProcessor::GetThreshold() const
	{
		return threshold;
	}

	void DeadzoneProcessor::SetThreshold(const float threshold)
	{
		this->threshold = std::clamp(threshold, 0.0f, MAX_THRESHOLD);
	}
}