#pragma once
#include "Core/Math/Color.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/UI/FontID.h"
#include "Data/Systems/UI/TextTexture.h"
#include "DF2D_API.h"
#include <string>
#include <vector>


namespace DF2D::Core
{
	class DF2D_API ITextBackend
	{
	public:
		virtual ~ITextBackend() = default;


		virtual Data::FontID LoadFont(const std::string& path, int size) = 0;

		virtual void SetFontStyle(Data::FontID font, Data::FontStyle style) = 0;

		virtual Data::TextTexture CreateTextTexture(Data::FontID font, const std::vector<std::string>& lines, Color color, bool centerText) = 0;
	};
}