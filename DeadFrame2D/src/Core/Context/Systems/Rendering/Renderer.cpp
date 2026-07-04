#include "Core/Context/Systems/Rendering/Renderer.h"
#include "Core/Context/Systems/Rendering/RenderPipeline.h"
#include <iostream>


namespace DF2D::Core
{
	using namespace DF2D::Data;


	Renderer::Renderer(std::unique_ptr<IRenderBackend> backend)
		: renderBackend(std::move(backend))
	{
		renderPipeline = std::make_unique<RenderPipeline>();

		std::cout << "[Info] Renderer successfully initialized." << std::endl;
	}

	Renderer::~Renderer()
	{
		renderPipeline.reset();

		renderBackend.reset();

		std::cout << "[Info] Renderer successfully destroyed." << std::endl;
	}


	void Renderer::BeginFrame()
	{

	}

	void Renderer::PreUpdate(float deltaTime)
	{

	}

	void Renderer::EndUpdate(float deltaTime)
	{

	}

	void Renderer::EndDraw()
	{

	}

	void Renderer::ClearAndPresentBuffer()
	{
		renderPipeline->Execute(*renderBackend, renderTasks);

		renderBackend->Present();

		for (auto& phaseMap : renderTasks)
		{
			phaseMap.clear();
		}
	}

	TextureID Renderer::CreateRenderTarget(int w, int h)
	{
		return renderBackend->CreateRenderTarget(w, h);
	}

	void Renderer::DestroyTexture(TextureID id)
	{
		renderBackend->DestroyTexture(id);
	}

	Color Renderer::GetDisplayColor()
	{
		return renderBackend->GetDisplayColor();
	}

	Vector2I Renderer::GetResolutionTarget()
	{
		return renderBackend->GetResolutionTarget();
	}

	void Renderer::SetViewport(RectI viewPort)
	{
		renderBackend->SetViewport(viewPort);
	}

	void Renderer::SetDisplayColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		renderBackend->SetDisplayColor(r, g, b, a);
	}

	void Renderer::SetResolutionTarget(Vector2I targetResolution)
	{
		renderBackend->SetResolutionTarget(targetResolution);
	}
}