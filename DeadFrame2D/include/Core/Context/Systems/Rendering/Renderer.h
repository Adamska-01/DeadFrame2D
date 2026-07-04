#pragma once
#include "Core/Context/Abstractions/ICoreSystem.h"
#include "Core/Context/Systems/Rendering/Abstractions/IRenderBackend.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Math/Color.h"
#include "Core/Math/Rect.h"
#include "Core/Math/Vector2.h"
#include "Data/Systems/Graphics/TextureID.h"
#include "Data/Systems/Rendering/RenderFlip.h"
#include "DF2D_API.h"
#include <cstdint>
#include <memory>
#include <optional>
#include <string>


namespace DF2D::Engine
{
	class Camera;
}


namespace DF2D::Core
{
	class RenderPipeline;


	class DF2D_API Renderer : public ICoreSystem, protected RenderSystem
	{
		friend class DeadFrameRuntime;
		friend class SystemInitializer;


	private:
		std::unique_ptr<IRenderBackend> renderBackend;

		std::unique_ptr<RenderPipeline> renderPipeline;


		void BeginFrame() override;

		void PreUpdate(float deltaTime) override;

		void EndUpdate(float deltaTime) override;

		void EndDraw() override;


		void ClearAndPresentBuffer();


	public:
		Renderer(std::unique_ptr<IRenderBackend> backend);

		~Renderer() override;

		Renderer(const Renderer&) = delete;

		Renderer(Renderer&&) = delete;

		Renderer& operator=(const Renderer&) = delete;

		Renderer& operator=(Renderer&&) = delete;


		Data::TextureID CreateRenderTarget(int w, int h);

		void DestroyTexture(Data::TextureID id);

		Color GetDisplayColor();

		Vector2I GetResolutionTarget();

		void SetViewport(RectI viewPort);

		void SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		void SetResolutionTarget(Vector2I targetResolution);
	};
}