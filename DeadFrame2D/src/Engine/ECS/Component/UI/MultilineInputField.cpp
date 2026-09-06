#include "Engine/ECS/Component/UI/MultilineInputField.h"


namespace DF2D::Engine
{
	using namespace DF2D::Data;


	MultilineInputField::MultilineInputField()
	{
	}


	UIElementType MultilineInputField::GetElementType() const
	{
		return UIElementType::TEXT_AREA;
	}
}