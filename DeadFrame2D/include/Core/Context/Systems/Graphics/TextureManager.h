#pragma once
#include "Core/Context/Abstractions/ISubSystem.h"
#include "DF2D_API.h"
#include <memory>
#include <string>
#include <unordered_map>


struct SDL_Texture;


namespace DeadFrame2D::Core
{
	class DF2D_API TextureManager : public ISubSystem
	{
		friend class SystemInitializer;


	private:
		static std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textureCache;


		TextureManager();

		~TextureManager() override;

		TextureManager(const TextureManager&) = delete;

		TextureManager(TextureManager&&) = delete;


		TextureManager& operator=(const TextureManager&) = delete;

		TextureManager& operator=(TextureManager&&) = delete;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


	public:
		static std::shared_ptr<SDL_Texture> LoadTexture(std::string_view filename);
	};
}