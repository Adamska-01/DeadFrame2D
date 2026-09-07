#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Layout/LayoutGroup.h"


namespace DF2D::Engine
{
	/**
	 * @brief Arranges its children in a single horizontal line.
	 */
	class DF2D_API HorizontalLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(HorizontalLayoutGroup, LayoutGroup);


	protected:
		void ApplyArrangement() override;


	public:
		HorizontalLayoutGroup();

		virtual ~HorizontalLayoutGroup() override = default;
	};
}