#pragma once
#include <RmlUi/Core/SystemInterface.h>


namespace DF2D::Core
{
	class IUIPlatform;
}


namespace DF2D::Internal
{
	/**
	 * @brief Supplies RmlUi with time, logging and the window-level services it asks the host for.
	 *
	 * Time comes from the engine clock rather than a private one so UI animations run on the same
	 * timeline as everything else, including when the engine scales or pauses it.
	 */
	class RmlSystemInterface : public Rml::SystemInterface
	{
	private:
		Core::IUIPlatform* platform;

		double elapsedSeconds = 0.0;


	public:
		RmlSystemInterface(Core::IUIPlatform* platform);

		~RmlSystemInterface() override;


		/**
		 * @brief Advances the clock RmlUi sees. Called once per frame by the backend.
		 *
		 * Fed unscaled delta on purpose: pausing or slowing the game must not freeze UI animations.
		 */
		void Advance(float deltaTime);


		double GetElapsedTime() override;

		bool LogMessage(Rml::Log::Type type, const Rml::String& message) override;

		void SetMouseCursor(const Rml::String& cursorName) override;

		void SetClipboardText(const Rml::String& text) override;

		void GetClipboardText(Rml::String& text) override;

		void ActivateKeyboard(Rml::Vector2f caretPosition, float lineHeight) override;

		void DeactivateKeyboard() override;
	};
}