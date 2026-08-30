#include "Factories/Products/Context/Systems/UI/RmlFileInterface.h"


namespace DF2D::Internal
{
	RmlFileInterface::~RmlFileInterface()
	{
	}


	Rml::FileHandle RmlFileInterface::Open(const Rml::String& path)
	{
		// Paths arriving here have already been through the engine's path resolution: engine assets come
		// in as a resolved ResolvedPath and game assets as working-directory-relative paths, the same
		// shape TextureManager takes. Everything else is a path RmlUi joined against one of those.
		auto* file = std::fopen(path.c_str(), "rb");

		return reinterpret_cast<Rml::FileHandle>(file);
	}

	void RmlFileInterface::Close(Rml::FileHandle file)
	{
		if (file == 0)
			return;

		std::fclose(reinterpret_cast<std::FILE*>(file));
	}

	size_t RmlFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
	{
		if (file == 0)
			return 0;

		return std::fread(buffer, 1, size, reinterpret_cast<std::FILE*>(file));
	}

	bool RmlFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
	{
		if (file == 0)
			return false;

		return std::fseek(reinterpret_cast<std::FILE*>(file), offset, origin) == 0;
	}

	size_t RmlFileInterface::Tell(Rml::FileHandle file)
	{
		if (file == 0)
			return 0;

		return static_cast<size_t>(std::ftell(reinterpret_cast<std::FILE*>(file)));
	}
}