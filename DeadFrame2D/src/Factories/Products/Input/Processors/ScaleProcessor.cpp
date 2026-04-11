#include "Factories/Products/Input/Processors/ScaleProcessor.h"


namespace DeadFrame2D::Factories
{
	using namespace DeadFrame2D::Core;

	using namespace Shared::Models;


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