#include "Core/Context/Systems/UI/UIManager.h"
#include <sstream>


namespace DF2D::Core
{
	using namespace DF2D::Data;


	UIManager::UIManager(std::unique_ptr<ITextBackend> backend)
		: backend(std::move(backend))
	{
	}

	UIManager::~UIManager()
	{
		fontCache.clear();

		backend.reset();
	}

	void UIManager::BeginFrame()
	{

	}

	void UIManager::PreUpdate(float deltaTime)
	{

	}

	void UIManager::EndUpdate(float deltaTime)
	{

	}

	void UIManager::EndDraw()
	{

	}

	FontID UIManager::LoadFont(std::string_view fontSource, int fontSize)
	{
		auto key = std::make_pair(std::string(fontSource), fontSize);
		auto it = fontCache.find(key);

		if (it != fontCache.end())
			return it->second;

		auto font = backend->LoadFont(key.first, fontSize);

		if (font == 0)
			return 0;

		fontCache[key] = font;

		return font;
	}

	void UIManager::SetFontStyle(std::string_view fontSource, int fontSize, FontStyle style)
	{
		auto font = LoadFont(fontSource, fontSize);

		if (font == 0)
			return;

		backend->SetFontStyle(font, style);
	}

	TextTexture UIManager::LoadText(std::string_view fontSource, int fontSize, std::string text, Color color, bool centerText)
	{
		if (text.empty())
			return {};

		auto font = LoadFont(fontSource, fontSize);

		if (font == 0)
			return {};

		auto ExpandTabs = [](const std::string& line, int tabWidth = 4) -> std::string
			{
				std::string result;
				auto col = 0;

				for (auto ch : line)
				{
					if (ch == '\t')
					{
						auto spaces = tabWidth - (col % tabWidth);
						result.append(spaces, ' ');
						col += spaces;
					}
					else
					{
						result += ch;
						++col;
					}
				}

				return result;
			};

		std::vector<std::string> lines;
		std::istringstream stream(text);
		std::string line;

		while (std::getline(stream, line))
		{
			if (line.empty())
			{
				line = " ";
			}

			lines.push_back(ExpandTabs(line));
		}

		return backend->CreateTextTexture(font, lines, color, centerText);
	}
}
