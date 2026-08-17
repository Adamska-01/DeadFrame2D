#include "Core/Context/Systems/Graphics/TextureManager.h"


namespace DF2D::Core
{
	TextureManager::TextureManager(std::unique_ptr<ITextureBackend> backend)
		: backend(std::move(backend))
	{
	}

	TextureManager::~TextureManager()
	{
		ClearCache();
	}


	void TextureManager::BeginFrame()
	{

	}

	void TextureManager::PreUpdate(float deltaTime)
	{

	}

	void TextureManager::EndUpdate(float deltaTime)
	{

	}

	void TextureManager::EndDraw()
	{

	}


	Data::TextureID TextureManager::LoadTexture(std::string_view filename)
	{
		auto filenameString = std::string(filename);

		auto it = filenameToID.find(filenameString);

		if (it != filenameToID.end())
			return it->second;

		auto id = backend->LoadFromFile(filenameString);

		if (id == 0)
			return Data::TextureID{};

		auto size = backend->GetTextureSize(id);
		textureSizes[id] = size;
		filenameToID[filenameString] = id;

		return id;
	}

	Vector2I TextureManager::GetTextureSize(Data::TextureID id)
	{
		auto it = textureSizes.find(id);

		return it != textureSizes.end() ? it->second : Vector2I::Zero;
	}

	void TextureManager::ClearCache()
	{
		for (const auto& [filename, id] : filenameToID)
		{
			backend->UnloadTexture(id);
		}

		filenameToID.clear();
		textureSizes.clear();
	}
}