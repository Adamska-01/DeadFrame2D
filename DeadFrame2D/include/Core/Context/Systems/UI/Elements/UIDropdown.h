#pragma once
#include "Data/Systems/UI/UIElementID.h"
#include "DF2D_API.h"
#include <string>


namespace DF2D::Core
{
	class UIManager;


	/**
	 * @brief The dropdown-only view of an element, reached through UIElement::AsDropdown.
	 *
	 * Kept apart from UIElement so that operations valid on exactly one kind of element are not
	 * offered on every kind. Selecting an option on a line of text is nonsense the type system can
	 * refuse outright, rather than something the backend discovers and quietly ignores.
	 *
	 * Options are not elements the engine hands out handles for: a dropdown builds its own popup and
	 * owns what is inside it, so they are addressed by index and managed from here.
	 */
	class DF2D_API UIDropdown
	{
		friend class UIElement;


	private:
		UIManager* manager = nullptr;

		Data::UIElementID id = 0;


		UIDropdown(UIManager* manager, Data::UIElementID id);


	public:
		UIDropdown() = default;


		bool IsValid() const;


		/** @brief Appends an option and returns its index, or -1 if it could not be added. */
		int AddOption(const std::string& text, const std::string& value);

		void ClearOptions();

		/** @brief Selects an option by index. An index outside the list clears the selection. */
		void SetSelection(int index);

		/** @brief The selected option's index, or -1 when nothing is selected. */
		int GetSelection() const;
	};
}