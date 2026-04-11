#include "Factories/Products/Input/Processors/NormalizeProcessor.h"


namespace DeadFrame2D::Factories
{
	using namespace DeadFrame2D::Core;

	using namespace Shared::Models;


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