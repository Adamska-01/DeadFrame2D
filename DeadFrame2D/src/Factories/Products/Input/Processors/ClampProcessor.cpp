#include "Factories/Products/Input/Processors/ClampProcessor.h"


namespace DeadFrame2D::Factories
{
	using namespace DeadFrame2D::Core;


	ClampProcessor::ClampProcessor(const float min, const float max)
		: min(min), 
		max(max)
	{
	}

	Shared::Models::ProcessorType ClampProcessor::GetType() const
	{
		return Shared::Models::ProcessorType::CLAMP;
	}

	void ClampProcessor::Process(float& value) const
	{
		value = std::clamp(value, min, max);
	}

	void ClampProcessor::Process(Vector2F& value) const
	{
		value.x = std::clamp(value.x, min, max);
		value.y = std::clamp(value.y, min, max);
	}

	float ClampProcessor::GetMin() const
	{
		return min;
	}

	float ClampProcessor::GetMax() const
	{
		return max;
	}

	void ClampProcessor::SetMin(const float min)
	{
		this->min = min;
	}

	void ClampProcessor::SetMax(const float max)
	{
		this->max = max;
	}
}