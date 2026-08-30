#pragma once
#include <cstdio>
#include <RmlUi/Core/FileInterface.h>


namespace DF2D::Internal
{
	/**
	 * @brief Routes every file RmlUi opens through the engine's own path resolution.
	 *
	 * Stylesheets reference other assets by relative path, so without this the UI would resolve paths
	 * by a different set of rules than the rest of the engine and break as soon as the mount prefix
	 * moves.
	 */
	class RmlFileInterface : public Rml::FileInterface
	{
	public:
		~RmlFileInterface() override;


		Rml::FileHandle Open(const Rml::String& path) override;

		void Close(Rml::FileHandle file) override;

		size_t Read(void* buffer, size_t size, Rml::FileHandle file) override;

		bool Seek(Rml::FileHandle file, long offset, int origin) override;

		size_t Tell(Rml::FileHandle file) override;
	};
}