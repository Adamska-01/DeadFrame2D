#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Layout/LayoutGroup.h"


namespace DF2D::Engine
{
	/**
	 * @brief Arranges its children in a single vertical line.
	 */
	class DF2D_API VerticalLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(VerticalLayoutGroup, LayoutGroup);


	protected:
		void ApplyArrangement() override;


	public:
		VerticalLayoutGroup();

		virtual ~VerticalLayoutGroup() override = default;
	};
}