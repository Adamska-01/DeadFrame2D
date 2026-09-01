#pragma once
#include "Core/Math/Rect.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/Pipeline/GeometryDrawList.h"
#include <cstdint>
#include <optional>
#include <RmlUi/Core/RenderInterface.h>
#include <unordered_map>
#include <vector>


namespace DF2D::Core
{
	class TextureManager;
}


namespace DF2D::Internal
{
	/**
	 * @brief RmlUi rendering implementation.
	 *
	 * Not exported: it derives from a library type that is not itself exported, and nothing outside
	 * this DLL ever names it -- the engine only ever sees Core::IUIBackend.
	 */
	class RmlRenderInterface : public Rml::RenderInterface
	{
	private:
		/**
		 * @brief A texture this interface handed to RmlUi, and whether releasing it should unload it.
		 *
		 * File-loaded textures come from the texture manager's shared path cache and may be in use by
		 * sprites elsewhere, so the UI only borrows them. Generated ones (glyph atlases) have no path,
		 * are uniquely owned here, and must be unloaded or they leak.
		 */
		struct BoundTexture
		{
			Data::TextureID id = 0;

			bool owned = false;
		};

		struct CompiledGeometry
		{
			std::vector<Data::Vertex2D> vertices;

			std::vector<uint32_t> indices;
		};


		Core::TextureManager* textureManager;

		std::unordered_map<Rml::CompiledGeometryHandle, CompiledGeometry> geometries;

		Rml::CompiledGeometryHandle nextGeometryHandle = 1;

		std::unordered_map<Rml::TextureHandle, BoundTexture> textures;

		Data::GeometryDrawList drawList;

		std::optional<Core::RectI> currentClipRect = std::nullopt;

		bool scissorEnabled = false;

		Core::RectI scissorRegion{};

		bool hasTransform = false;

		float transform[6] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };


		void UpdateClipRect();


	public:
		RmlRenderInterface(Core::TextureManager* textureManager);

		~RmlRenderInterface() override;

		RmlRenderInterface(const RmlRenderInterface&) = delete;

		RmlRenderInterface(RmlRenderInterface&&) = delete;

		RmlRenderInterface& operator=(const RmlRenderInterface&) = delete;

		RmlRenderInterface& operator=(RmlRenderInterface&&) = delete;


		/** @brief Discards anything recorded so far, before a context renders into this interface. */
		void BeginRecording();

		/** @brief Hands over everything recorded since BeginRecording. */
		Data::GeometryDrawList TakeRecording();


		Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;

		void RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) override;

		void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;

		Rml::TextureHandle LoadTexture(Rml::Vector2i& textureDimensions, const Rml::String& source) override;

		Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i sourceDimensions) override;

		void ReleaseTexture(Rml::TextureHandle texture) override;

		void EnableScissorRegion(bool enable) override;

		void SetScissorRegion(Rml::Rectanglei region) override;

		void SetTransform(const Rml::Matrix4f* newTransform) override;
	};
}