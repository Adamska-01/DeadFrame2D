#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/UI/Abstractions/ITextBackend.h"
#include "Core/Math/Color.h"
#include "Data/Components/UI/Text/FontStyle.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/UI/FontID.h"
#include "Data/Systems/UI/TextTexture.h"
#include "DF2D_API.h"
#include "Utilities/Hashing/PairHash.h"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>


namespace DF2D::Core
{
	class DF2D_API UIManager : public ICoreSystem
	{
	private:
		std::unique_ptr<ITextBackend> backend;

		std::unordered_map<std::pair<std::string, int>, Data::FontID, Utilities::PairHash> fontCache;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		UIManager(std::unique_ptr<ITextBackend> backend);

		~UIManager() override;

		UIManager(const UIManager&) = delete;

		UIManager(UIManager&&) = delete;

		UIManager& operator=(const UIManager&) = delete;

		UIManager& operator=(UIManager&&) = delete;


		Data::FontID LoadFont(std::string_view fontSource, int fontSize);

		void SetFontStyle(std::string_view fontSource, int fontSize, Data::FontStyle style);

		Data::TextTexture LoadText(std::string_view fontSource, int fontSize, std::string text, Color color, bool centerText = false);
	};
}