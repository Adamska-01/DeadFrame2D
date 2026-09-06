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

		const std::string& GetText() const;
	};
}