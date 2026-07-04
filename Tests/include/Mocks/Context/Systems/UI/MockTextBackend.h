#pragma once
#include "Core/Context/Systems/UI/Abstractions/ITextBackend.h"


struct MockTextBackend : DF2D::Core::ITextBackend
{
	// Configurable results
	DF2D::Data::FontID nextFontId{1};

	bool failNextFontLoad{false};

	DF2D::Data::TextureID nextTextureId{100};

	DF2D::Core::Vector2I nextTextureSize{320, 48};


	// Call tracking
	int loadFontCount{0};

	int setFontStyleCount{0};

	int createTextTextureCount{0};

	std::string lastFontPath;

	int lastFontSize{0};

	DF2D::Data::FontID lastStyledFont{0};

	DF2D::Data::FontStyle lastStyle{DF2D::Data::FontStyle::NORMAL};

	DF2D::Data::FontID lastTextFont{0};

	std::vector<std::string> lastLines;

	DF2D::Core::Color lastColor{};

	bool lastCenterText{false};


	DF2D::Data::FontID LoadFont(const std::string& path, int size) override
	{
		loadFontCount++;
		lastFontPath = path;
		lastFontSize = size;

		if (failNextFontLoad)
			return 0;

		return nextFontId++;
	}

	void SetFontStyle(DF2D::Data::FontID font, DF2D::Data::FontStyle style) override
	{
		setFontStyleCount++;
		lastStyledFont = font;
		lastStyle = style;
	}

	DF2D::Data::TextTexture CreateTextTexture(DF2D::Data::FontID font, const std::vector<std::string>& lines, DF2D::Core::Color color, bool centerText) override
	{
		createTextTextureCount++;
		lastTextFont = font;
		lastLines = lines;
		lastColor = color;
		lastCenterText = centerText;

		return { nextTextureId, nextTextureSize };
	}
};