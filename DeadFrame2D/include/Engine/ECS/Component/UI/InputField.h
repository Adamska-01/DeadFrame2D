#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <string>
#include <string_view>


namespace DF2D::Engine
{
	/**
	 * @brief An editable line or block of text.
	 *
	 * Caret movement, selection, clipboard and undo are handled by the UI backend.
	 */
	class DF2D_API InputField : public IInteractableUI
	{
		TYPE_INFO(InputField, IInteractableUI);


	private:
		std::string text;

		std::string placeholder;

		int maxLength = -1;

		bool multiline = false;


		void ApplyText();

		void ApplyPlaceholder();

		void ApplyMaxLength();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;

		void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		InputField();

		virtual ~InputField() override = default;


		/** @brief Fired on every edit, including ones made by SetText. */
		Utilities::MulticastDelegate<const std::string&> OnValueChanged;

		/** @brief Fired when the player commits the value, by pressing Enter in a single-line field. */
		Utilities::MulticastDelegate<const std::string&> OnSubmit;


		void SetText(std::string_view value);

		void SetPlaceholder(std::string_view value);

		void SetMaxLength(int length);

		/**
		 * @brief Switches between a single line and a block of text.
		 *
		 * Takes effect only before the component is initialised, because the two are different kinds of
		 * element and an element's kind is fixed when it is built. Scenes set this while building, which
		 * runs first, so the ordinary authoring path is unaffected.
		 */
		void SetMultiline(bool value);


		const std::string& GetText() const;

		bool IsMultiline() const;
	};
}