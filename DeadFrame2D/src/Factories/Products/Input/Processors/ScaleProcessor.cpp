#include "Factories/Products/Input/Processors/ScaleProcessor.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Models;


	ScaleProcessor::ScaleProcessor(const float factor)
		: factor(factor)
	{
	}

	ProcessorType ScaleProcessor::GetType() const
	{
		return ProcessorType::SCALE;
	}

	void ScaleProcessor::Process(float& value) const
	{
		value *= factor;
	}

	void ScaleProcessor::Process(Vector2F& value) const
	{
		value.x *= factor;
		value.y *= factor;
	}

	float ScaleProcessor::GetFactor() const
	{
		return factor;
	}

	void ScaleProcessor::SetFactor(const float factor)
	{
		this->factor = factor;
	}
}