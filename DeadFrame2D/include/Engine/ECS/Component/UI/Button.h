#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include <string>
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief A clickable button.
	 *
	 * Carries no visual state of its own. Hovered, pressed and disabled looks come from the stylesheet
	 * selecting on the element's own states, so restyling a button never means recompiling.
	 */
	class DF2D_API Button : public IInteractableUI
	{
		TYPE_INFO(Button, IInteractableUI);


	private:
		std::string label;


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;


	public:
		Button();

		virtual ~Button() override = default;


		/** @brief Sets the button's caption. Safe to call before the component is initialised. */
		void SetText(std::string_view value);

		const std::string& GetText() const;
	};
}