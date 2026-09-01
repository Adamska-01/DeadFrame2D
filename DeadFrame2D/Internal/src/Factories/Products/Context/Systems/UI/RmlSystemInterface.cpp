#include "Converters/UI/RmlConversions.h"
#include "Core/Context/Systems/UI/Abstractions/IUIPlatform.h"
#include "Factories/Products/Context/Systems/UI/RmlSystemInterface.h"
#include <iostream>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Internal::RmlConversions;


	RmlSystemInterface::RmlSystemInterface(IUIPlatform* platform)
		: platform(platform)
	{
	}

	RmlSystemInterface::~RmlSystemInterface()
	{
	}


	void RmlSystemInterface::Advance(float deltaTime)
	{
		elapsedSeconds += static_cast<double>(deltaTime);
	}

	double RmlSystemInterface::GetElapsedTime()
	{
		return elapsedSeconds;
	}

	bool RmlSystemInterface::LogMessage(Rml::Log::Type type, const Rml::String& message)
	{
		switch (type)
		{
		case Rml::Log::LT_ERROR:
		case Rml::Log::LT_ASSERT:
			std::cerr << "[UI] " << message << std::endl;
			break;

		case Rml::Log::LT_WARNING:
			std::cerr << "[UI][Warning] " << message << std::endl;
			break;

		default:
			// Info and debug chatter is very noisy during layout; keep it out of the engine log for now...
			// A proper logging system will be implemented in the future...
			break;
		}

		return true;
	}

	void RmlSystemInterface::SetMouseCursor(const Rml::String& cursorName)
	{
		if (platform == nullptr)
			return;

		platform->SetCursor(ToCursorType(cursorName));
	}

	void RmlSystemInterface::SetClipboardText(const Rml::String& text)
	{
		if (platform == nullptr)
			return;

		platform->SetClipboardText(text);
	}

	void RmlSystemInterface::GetClipboardText(Rml::String& text)
	{
		if (platform == nullptr)
			return;

		text = platform->GetClipboardText();
	}

	void RmlSystemInterface::ActivateKeyboard(Rml::Vector2f caretPosition, float lineHeight)
	{
		if (platform == nullptr)
			return;

		platform->StartTextInput(RectI
			{
				.x = static_cast<int>(caretPosition.x),
				.y = static_cast<int>(caretPosition.y),
				.w = 1,
				.h = static_cast<int>(lineHeight)
			});
	}

	void RmlSystemInterface::DeactivateKeyboard()
	{
		if (platform == nullptr)
			return;

		platform->StopTextInput();
	}
}