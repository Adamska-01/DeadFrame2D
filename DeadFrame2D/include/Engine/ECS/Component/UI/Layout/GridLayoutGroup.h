#pragma once
#include "Data/Components/UI/Layout/GridFlow.h"
#include "DF2D_API.h"
#include "Engine/ECS/Component/UI/Layout/LayoutGroup.h"


namespace DF2D::Engine
{
	/**
	 * @brief Arranges its children in lines that wrap onto the next one when they run out of room.
	 *
	 * Sets no cell size. A child's size comes from its own RectTransform or LayoutElement, so for even
	 * cells give the children even sizes.
	 */
	class DF2D_API GridLayoutGroup : public LayoutGroup
	{
		TYPE_INFO(GridLayoutGroup, LayoutGroup);


	private:
		Data::GridFlow flow = Data::GridFlow::ROWS;


	protected:
		void ApplyArrangement() override;


	public:
		GridLayoutGroup();

		virtual ~GridLayoutGroup() override = default;


		/** @brief Whether children fill a row before wrapping downwards, or a column before wrapping across. */
		void SetFlow(Data::GridFlow value);

		Data::GridFlow GetFlow() const;
	};
}