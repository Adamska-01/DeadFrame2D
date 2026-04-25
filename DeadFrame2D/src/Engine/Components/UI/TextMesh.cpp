#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Components/UI/TextMeshComponentModel.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/UI/Canvas.h"
#include "Engine/Components/UI/TextMesh.h"
#include "Engine/Entity/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DeadFrame2D::Engine
{
	using namespace DeadFrame2D::Core;
	using namespace DeadFrame2D::Data;
	using namespace DeadFrame2D::Utilities;


	TextMesh::TextMesh(const TextMeshComponentModel& textMeshConfiguration)
		: text(textMeshConfiguration.text)
	{
		font = UIManager::LoadFont(textMeshConfiguration.fontSource, textMeshConfiguration.fontSize);
	
		initialObjectScale = textMeshConfiguration.textObjectInitialScale;

		centered = false;
		fontSize = 30;

		SetText(text);
		SetTextColor(textMeshConfiguration.textColor);
		SetFontStyle(textMeshConfiguration.fontStyle);
		SetIsCentered(textMeshConfiguration.isCentered);
	}

	void TextMesh::Init()
	{
		UIComponent::Init();

		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		transform->SetLocalScale(initialObjectScale);
	}

	void TextMesh::Draw()
	{
		UIComponent::Draw();

		auto destRect = GetTextDestRect();

		renderTask.renderPhase = parentCanvas->GetRenderMode() == CanvasRenderMode::SCREEN_SPACE_CAMERA ? RenderPhase::SCREEN_SPACE_CAMERA_UI : RenderPhase::SCREEN_SPACE_OVERLAY_UI;
		renderTask.sortOrder = parentCanvas->GetSortOrder();
		renderTask.canvas = parentCanvas;
		renderTask.renderData = SpriteRenderData
		{
			.texture = textTexture.get(),
			.srcRect = std::nullopt,
			.destRect = destRect,
			.rotation = transform->GetWorldRotation()
		};

		RenderSystem::Submit(renderTask);
	}

	void TextMesh::SetFontSize(unsigned int newFontSize)
	{
		fontSize = newFontSize;
	}

	void TextMesh::SetTextColor(SDL_Color newColor)
	{
		color = newColor;

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	void TextMesh::SetFontStyle(FontStyle newFontStyle)
	{
		TTF_SetFontStyle(font.get(), newFontStyle);

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	void TextMesh::SetText(std::string newText)
	{
		text = newText;

		textTexture = UIManager::LoadText(font, text, color, centered);

		auto width = 0, height = 0;
		SDL_QueryTexture(textTexture.get(), NULL, NULL, &width, &height);

		widgetSize.x = static_cast<float>(width);
		widgetSize.y = static_cast<float>(height);
	}

	void TextMesh::SetIsCentered(bool isCentered)
	{
		centered = isCentered;

		textTexture = UIManager::LoadText(font, text, color, centered);
	}

	std::string TextMesh::GetText()
	{
		return text;
	}

	SDL_FRect TextMesh::GetTextDestRect()
	{
		auto currentPosition = transform->GetWorldPosition();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		return SDL_FRect
		{
			currentPosition.x - (scaledSize.x * anchorVector.x),
			currentPosition.y - (scaledSize.y * anchorVector.y),
			scaledSize.x,
			scaledSize.y
		};
	}
}