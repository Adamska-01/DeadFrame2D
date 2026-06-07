#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "DF2D_API.h"
#include <string>
#include <unordered_map>


namespace DF2D::Core
{
	class DF2D_API TextureManager : public ICoreSystem
	{
	private:
		std::unordered_map<std::string, Data::TextureID> filenameToID;

		std::unordered_map<Data::TextureID, Vector2I> textureSizes;

		std::unique_ptr<ITextureBackend> backend;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		Data::TextureID LoadTextureImpl(const std::string& filename);


	public:
		TextureManager(std::unique_ptr<ITextureBackend> backend);

		~TextureManager() override;

		TextureManager(const TextureManager&) = delete;

		TextureManager(TextureManager&&) = delete;


		TextureManager& operator=(const TextureManager&) = delete;

		TextureManager& operator=(TextureManager&&) = delete;


		Data::TextureID LoadTexture(std::string_view filename);

		Vector2I GetTextureSize(Data::TextureID id);

		void ClearCache();
	};
}