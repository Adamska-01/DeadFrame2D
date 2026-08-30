#pragma once
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/Resolvers/Abstractions/RenderResolver.h"
#include "Data/Systems/Rendering/Pipeline/Shapes/GeometryRenderData.h"
#include "Engine/ECS/Component/Rendering/Camera/Camera.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include <optional>


namespace DF2D::Core
{
	template<>
	struct RenderResolver<Data::GeometryRenderData>
	{
		static void Render(
			IRenderBackend& renderBackend,
			const Data::GeometryRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera,
			bool requiresScreenSpaceConversion)
		{
			if (renderData.drawList == nullptr)
				return;

			const auto& drawList = *renderData.drawList;

			// Commands carry their own scissor state and rely on painter's-algorithm ordering, so they
			// are replayed exactly as submitted. Producers have already positioned every vertex in
			// target space, which is why the camera is not consulted here.
			for (const auto& command : drawList.commands)
			{
				if (command.indexCount == 0 || command.vertexCount == 0)
					continue;

				if (command.vertexOffset + command.vertexCount > drawList.vertices.size())
					continue;

				if (command.indexOffset + command.indexCount > drawList.indices.size())
					continue;

				renderBackend.SetClipRect(command.clipRect);

				renderBackend.DrawGeometry(
					command.texture,
					std::span<const Data::Vertex2D>(drawList.vertices).subspan(command.vertexOffset, command.vertexCount),
					std::span<const uint32_t>(drawList.indices).subspan(command.indexOffset, command.indexCount),
					command.translation,
					command.blendMode);
			}

			// Clipping is renderer state, not per-draw state; leaving the last command's rectangle
			// applied would silently clip whatever the pipeline draws next.
			renderBackend.SetClipRect(std::nullopt);
		}

		static std::optional<Data::GeometryRenderData> Cull(
			const Data::GeometryRenderData& renderData,
			Engine::ComponentHandle<Engine::Camera> camera)
		{
			// A draw list is opaque to the pipeline and its producer has already culled internally, so
			// it is either drawn whole or not at all.
			return renderData;
		}
	};
}