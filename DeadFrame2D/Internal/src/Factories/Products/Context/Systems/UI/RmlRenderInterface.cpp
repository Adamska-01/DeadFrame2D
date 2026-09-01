#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Factories/Products/Context/Systems/UI/RmlRenderInterface.h"
#include <RmlUi/Core/Types.h>


namespace DF2D::Internal
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;


	RmlRenderInterface::RmlRenderInterface(TextureManager* textureManager)
		: textureManager(textureManager)
	{
	}

	RmlRenderInterface::~RmlRenderInterface()
	{
		// RmlUi releases its own textures on shutdown, but the interface may outlive that if the host
		// tears down out of order; dropping ours here keeps the texture manager's books balanced.
		if (textureManager != nullptr)
		{
			for (const auto& [handle, bound] : textures)
			{
				if (bound.owned)
				{
					textureManager->UnloadTexture(bound.id);
				}
			}
		}

		textures.clear();
		geometries.clear();
	}


	void RmlRenderInterface::UpdateClipRect()
	{
		currentClipRect = scissorEnabled
			? std::optional<RectI>(scissorRegion)
			: std::nullopt;
	}

	void RmlRenderInterface::BeginRecording()
	{
		drawList.vertices.clear();
		drawList.indices.clear();
		drawList.commands.clear();

		scissorEnabled = false;
		currentClipRect = std::nullopt;
		hasTransform = false;
	}

	GeometryDrawList RmlRenderInterface::TakeRecording()
	{
		auto recorded = std::move(drawList);

		drawList = GeometryDrawList();

		return recorded;
	}


	Rml::CompiledGeometryHandle RmlRenderInterface::CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices)
	{
		auto handle = nextGeometryHandle++;

		auto& compiled = geometries[handle];

		compiled.vertices.resize(vertices.size());

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const auto& source = vertices[i];

			// RmlUi orders its vertex as position/colour/tex_coord while the engine orders it
			// position/texCoord/color, so this is a field-by-field copy rather than a memcpy.
			compiled.vertices[i] = Vertex2D
			{
				.position = Vector2F(source.position.x, source.position.y),
				.texCoord = Vector2F(source.tex_coord.x, source.tex_coord.y),
				.color = Color{ source.colour.red, source.colour.green, source.colour.blue, source.colour.alpha }
			};
		}

		compiled.indices.resize(indices.size());

		for (size_t i = 0; i < indices.size(); ++i)
		{
			compiled.indices[i] = static_cast<uint32_t>(indices[i]);
		}

		return handle;
	}

	void RmlRenderInterface::RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture)
	{
		auto it = geometries.find(geometry);

		if (it == geometries.end())
			return;

		const auto& compiled = it->second;

		if (compiled.vertices.empty() || compiled.indices.empty())
			return;

		auto command = GeometryCommand
		{
			.texture = 0,
			.vertexOffset = static_cast<uint32_t>(drawList.vertices.size()),
			.vertexCount = static_cast<uint32_t>(compiled.vertices.size()),
			.indexOffset = static_cast<uint32_t>(drawList.indices.size()),
			.indexCount = static_cast<uint32_t>(compiled.indices.size()),
			.translation = Vector2F(translation.x, translation.y),
			.clipRect = currentClipRect,
			// RmlUi emits premultiplied vertex colours; compositing them with straight alpha would
			// multiply by alpha a second time and darken every antialiased edge.
			.blendMode = BlendMode::PREMULTIPLIED_ALPHA
		};

		auto textureIt = textures.find(texture);

		if (textureIt != textures.end())
		{
			command.texture = textureIt->second.id;
		}

		if (!hasTransform)
		{
			drawList.vertices.insert(drawList.vertices.end(), compiled.vertices.begin(), compiled.vertices.end());
		}
		else
		{
			// The render backend has no matrix stack, so an active transform is baked into the
			// positions here. Only the affine 2D part is applied: perspective is dropped, which is the
			// documented limit of the current backend.
			for (const auto& vertex : compiled.vertices)
			{
				auto transformed = vertex;

				transformed.position = Vector2F(
					transform[0] * vertex.position.x + transform[2] * vertex.position.y + transform[4],
					transform[1] * vertex.position.x + transform[3] * vertex.position.y + transform[5]);

				drawList.vertices.push_back(transformed);
			}
		}

		drawList.indices.insert(drawList.indices.end(), compiled.indices.begin(), compiled.indices.end());

		drawList.commands.push_back(command);
	}

	void RmlRenderInterface::ReleaseGeometry(Rml::CompiledGeometryHandle geometry)
	{
		geometries.erase(geometry);
	}


	Rml::TextureHandle RmlRenderInterface::LoadTexture(Rml::Vector2i& textureDimensions, const Rml::String& source)
	{
		if (textureManager == nullptr)
			return 0;

		// Going through the texture manager rather than loading directly means UI textures share the
		// engine's cache and path handling, and show up in the same registry as everything else.
		auto id = textureManager->LoadTexture(source);

		if (id == 0)
			return 0;

		auto size = textureManager->GetTextureSize(id);
		textureDimensions = Rml::Vector2i(size.x, size.y);

		textures[static_cast<Rml::TextureHandle>(id)] = BoundTexture{ .id = id, .owned = false };

		return static_cast<Rml::TextureHandle>(id);
	}

	Rml::TextureHandle RmlRenderInterface::GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i sourceDimensions)
	{
		if (textureManager == nullptr)
			return 0;

		auto id = textureManager->CreateTexture(
			std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(source.data()), source.size()),
			Vector2I(sourceDimensions.x, sourceDimensions.y));

		if (id == 0)
			return 0;

		textures[static_cast<Rml::TextureHandle>(id)] = BoundTexture{ .id = id, .owned = true };

		return static_cast<Rml::TextureHandle>(id);
	}

	void RmlRenderInterface::ReleaseTexture(Rml::TextureHandle texture)
	{
		auto it = textures.find(texture);

		if (it == textures.end())
			return;

		// Only atlases this interface generated are ours to destroy; a file texture is still owned by
		// the texture manager's cache and may back a sprite somewhere else in the scene.
		if (it->second.owned && textureManager != nullptr)
		{
			textureManager->UnloadTexture(it->second.id);
		}

		textures.erase(it);
	}


	void RmlRenderInterface::EnableScissorRegion(bool enable)
	{
		scissorEnabled = enable;

		UpdateClipRect();
	}

	void RmlRenderInterface::SetScissorRegion(Rml::Rectanglei region)
	{
		scissorRegion = RectI
		{
			.x = region.Left(),
			.y = region.Top(),
			.w = region.Width(),
			.h = region.Height()
		};

		UpdateClipRect();
	}

	void RmlRenderInterface::SetTransform(const Rml::Matrix4f* newTransform)
	{
		if (newTransform == nullptr)
		{
			hasTransform = false;

			return;
		}

		hasTransform = true;

		// Keep only the 2D affine part: the two basis vectors and the translation column.
		const auto column0 = newTransform->GetColumn(0);
		const auto column1 = newTransform->GetColumn(1);
		const auto column3 = newTransform->GetColumn(3);

		transform[0] = column0.x;
		transform[1] = column0.y;
		transform[2] = column1.x;
		transform[3] = column1.y;
		transform[4] = column3.x;
		transform[5] = column3.y;
	}
}