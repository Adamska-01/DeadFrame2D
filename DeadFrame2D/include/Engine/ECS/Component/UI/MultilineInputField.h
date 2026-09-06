#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/InputField.h"


namespace DF2D::Engine
{
	/**
	 * @brief An editable block of text, wrapping and scrolling over several lines.
	 */
	class DF2D_API MultilineInputField : public InputField
	{
		TYPE_INFO(MultilineInputField, InputField);


	protected:
		Data::UIElementType GetElementType() const override;


	public:
		MultilineInputField();

		virtual ~MultilineInputField() override = default;
	};
}
