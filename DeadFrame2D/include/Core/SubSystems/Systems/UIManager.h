#pragma once
#include "Core/SubSystems/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include "Utilities/Hashing/PairHash.h"
#include <memory>
#include <SDL_ttf.h>
#include <string>
#include <unordered_map>


struct SDL_Texture;


namespace DeadFrame2D::Core
{
	class DF2D_API UIManager : public ISubSystem
	{
		friend class SubSystems;


	private:
		UIManager();

		virtual ~UIManager() override;

		UIManager(const UIManager&) = delete;

		UIManager(UIManager&&) = delete;


		UIManager& operator=(const UIManager&) = delete;

		UIManager& operator=(UIManager&&) = delete;


		static std::unordered_map<std::pair<std::string, int>, std::shared_ptr<TTF_Font>, DeadFrame2D::Utilities::PairHash> fontCache;


		virtual void Update(float deltaTime) override;

		virtual void BeginFrame() override;

		virtual void EndUpdate() override;

		virtual void EndDraw() override;


	public:
		static std::shared_ptr<TTF_Font> LoadFont(std::string_view textSource, int fontsize);

		static std::shared_ptr<SDL_Texture> LoadText(std::shared_ptr<TTF_Font> font, std::string text, SDL_Color color, bool centerText = false);
	};
}