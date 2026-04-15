#include "Factories/Products/Input/Processors/InvertProcessor.h"


namespace DeadFrame2D::Factories
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Models;


	ProcessorType InvertProcessor::GetType() const
	{
		return ProcessorType::INVERT;
	}

	void InvertProcessor::Process(float& value) const
	{
		value = -value;
	}

	void InvertProcessor::Process(Vector2F& value) const
	{
		value.x = -value.x;
		value.y = -value.y;
	}
}