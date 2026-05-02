#include "Factories/Products/Input/Processors/NormalizeProcessor.h"


namespace DF2D::Factories
{
	using namespace DF2D::Core;
	using namespace DF2D::Models;


	ProcessorType NormalizeProcessor::GetType() const
	{
		return ProcessorType::NORMALIZE;
	}

	void NormalizeProcessor::Process(float& value) const
	{
		// No normalization for float values
	}

	void NormalizeProcessor::Process(Vector2F& value) const
	{
		value = value.Normalize();
	}
}