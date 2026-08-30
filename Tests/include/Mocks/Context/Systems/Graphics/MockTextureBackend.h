#pragma once
#include "Core/Context/Systems/Graphics/Abstractions/ITextureBackend.h"
#include <cstdint>
#include <span>
#include <unordered_map>


struct MockTextureBackend : DF2D::Core::ITextureBackend
{
	// Configurable results
	DF2D::Data::TextureID nextId{1};

	bool failNextLoad{false};

	bool failNextCreate{false};

	DF2D::Core::Vector2I nextSize{64, 64};


	// Call tracking
	int loadCount{0};

	int unloadCount{0};

	int createFromPixelsCount{0};

	DF2D::Core::Vector2I lastCreatedSize{};

	size_t lastCreatedByteCount{0};

	std::string lastLoadedFile;

	std::unordered_map<DF2D::Data::TextureID, DF2D::Core::Vector2I> sizes;


	DF2D::Data::TextureID LoadFromFile(const std::string& file) override
	{
		lastLoadedFile = file;
		loadCount++;

		if (failNextLoad) return 0;

		auto id = nextId++;
		sizes[id] = nextSize;

		return id;
	}

	DF2D::Data::TextureID CreateFromPixels(std::span<const uint8_t> rgba, DF2D::Core::Vector2I size) override
	{
		createFromPixelsCount++;
		lastCreatedSize = size;
		lastCreatedByteCount = rgba.size();

		if (failNextCreate) return 0;

		auto id = nextId++;
		sizes[id] = size;

		return id;
	}

	void UnloadTexture(DF2D::Data::TextureID id) override
	{
		unloadCount++;
		sizes.erase(id);
	}

	DF2D::Core::Vector2I GetTextureSize(DF2D::Data::TextureID id) override
	{
		auto it = sizes.find(id);

		return it != sizes.end() ? it->second : DF2D::Core::Vector2I::Zero;
	}
};