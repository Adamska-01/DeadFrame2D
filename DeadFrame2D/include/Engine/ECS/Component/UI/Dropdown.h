#pragma once
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Core/UI/Abstractions/IInteractableUI.h"
#include "Utilities/Delegates/MulticastDelegate.h"
#include <string>
#include <string_view>
#include <vector>


namespace DF2D::Engine
{
	/**
	 * @brief A closed list of options, one of which is selected.
	 *
	 * The options are not GameObjects. A dropdown builds its own popup and owns what is inside it, so
	 * the engine addresses options by index rather than handing out a handle per option.
	 */
	class DF2D_API Dropdown : public IInteractableUI
	{
		TYPE_INFO(Dropdown, IInteractableUI);


	private:
		std::vector<std::string> options;

		int selectedIndex;


		void ApplyOptions();

		void ApplySelection();


	protected:
		Data::UIElementType GetElementType() const override;

		void OnElementCreated() override;

		void OnInteraction(Data::UIEventType eventType, const Data::UIEventPayload& payload) override;


	public:
		Dropdown();

		virtual ~Dropdown() override = default;


		/** @brief Fired when the selection changes, carrying the new index, or -1 for no selection. */
		Utilities::MulticastDelegate<int> OnValueChanged;


		/** @brief Replaces every option, selecting the first one if there is one. */
		void SetOptions(std::vector<std::string> values);

		/** @brief Appends an option and returns its index. */
		int AddOption(std::string_view value);

		/** @brief Removes every option, leaving nothing selected. */
		void ClearOptions();

		/** @brief Selects an option. An index outside the list clears the selection. */
		void SetSelectedIndex(int index);


		int GetSelectedIndex() const;

		/** @brief The selected option's text, or empty when nothing is selected. */
		const std::string& GetSelectedOption() const;

		const std::vector<std::string>& GetOptions() const;
	};
}