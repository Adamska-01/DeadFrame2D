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
		auto source = std::string(filename);

		auto cached = sourceToID.find(source);

		if (cached != sourceToID.end())
			return cached->second;

		auto id = backend->LoadFromFile(source);

		if (id == 0)
			return Data::TextureID{};

		textures[id] = TextureRecord
		{
			.size = backend->GetTextureSize(id),
			.source = source
		};
		sourceToID[source] = id;

		return id;
	}

	Data::TextureID TextureManager::CreateTexture(std::span<const uint8_t> rgba, Vector2I size)
	{
		auto id = backend->CreateFromPixels(rgba, size);

		if (id == 0)
			return Data::TextureID{};

		// No source path: generated textures are owned by their caller and never deduped.
		textures[id] = TextureRecord
		{
			.size = size,
			.source = {}
		};

		return id;
	}

	std::string TextureManager::RegisterGeneratedTexture(std::span<const uint8_t> rgba, Vector2I size)
	{
		auto id = CreateTexture(rgba, size);

		if (id == 0)
			return {};

		// The id is already unique for the manager's lifetime, so building the path from it needs no
		// separate counter and can never collide with one a caller picks by hand.
		auto path = "generated://" + std::to_string(id);

		textures[id].source = path;
		sourceToID[path] = id;

		return path;
	}

	void TextureManager::UnloadTexture(Data::TextureID id)
	{
		auto it = textures.find(id);

		if (it == textures.end())
			return;

		if (!it->second.source.empty())
		{
			sourceToID.erase(it->second.source);
		}

		backend->UnloadTexture(id);

		textures.erase(it);
	}

	Vector2I TextureManager::GetTextureSize(Data::TextureID id)
	{
		auto it = textures.find(id);

		return it != textures.end() ? it->second.size : Vector2I::Zero;
	}

	void TextureManager::ClearCache()
	{
		for (const auto& [id, record] : textures)
		{
			backend->UnloadTexture(id);
		}

		textures.clear();
		sourceToID.clear();
	}
}