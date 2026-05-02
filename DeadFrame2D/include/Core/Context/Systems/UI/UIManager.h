#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "DF2D_API.h"
#include "Utilities/Hashing/PairHash.h"
#include <memory>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>


struct SDL_Texture;


namespace DF2D::Core
{
	class DF2D_API UIManager : public ICoreSystem
	{
		friend class SystemInitializer;


	private:
		UIManager();

		~UIManager() override;

		UIManager(const UIManager&) = delete;

		UIManager(UIManager&&) = delete;


		UIManager& operator=(const UIManager&) = delete;

		UIManager& operator=(UIManager&&) = delete;


		static std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, Utilities::PairHash> fontCache;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static std::shared_ptr<TTF_Font> LoadFont(std::string_view textSource, int fontsize);

		static std::shared_ptr<SDL_Texture> LoadText(std::shared_ptr<TTF_Font> font, std::string text, SDL_Color color, bool centerText = false);
	};
}