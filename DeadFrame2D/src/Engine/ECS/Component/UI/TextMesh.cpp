#include "Core/Context/Systems/Graphics/TextureManager.h"
#include "Core/Context/Systems/Rendering/RenderSystem.h"
#include "Core/Context/Systems/UI/UIManager.h"
#include "Data/Blueprints/UI/Text/TextMeshComponentModel.h"
#include "Engine/ECS/Component/Transform.h"
#include "Engine/ECS/Component/UI/Canvas.h"
#include "Engine/ECS/Component/UI/TextMesh.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Core;
	using namespace DF2D::Data;
	using namespace DF2D::Utilities;


	TextMesh::TextMesh(const TextMeshComponentModel& textMeshConfiguration)
		: uiManager(nullptr),
		text(textMeshConfiguration.text)
	{
		fontSource = std::string(textMeshConfiguration.fontSource);

		initialObjectScale = textMeshConfiguration.textObjectInitialScale;

		color = textMeshConfiguration.textColor;

		fontStyle = textMeshConfiguration.fontStyle;

		centered = textMeshConfiguration.isCentered;

		fontSize = textMeshConfiguration.fontSize;
	}

	void TextMesh::Init()
	{
		UIComponent::Init();

		uiManager = Guard::AgainstNullAssignment(GetGameObject()->CoreContext().uiManager, NAME_OF(uiManager));
		transform = Guard::AgainstNullAssignment(GetGameObject()->GetTransform(), NAME_OF(transform));

		transform->SetLocalScale(initialObjectScale);

		RebuildTextTexture();
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
			.texture = textTexture,
			.srcRect = std::nullopt,
			.destRect = destRect,
			.rotation = transform->GetWorldRotation()
		};

		RenderSystem::Submit(renderTask);
	}

	void TextMesh::RebuildTextTexture()
	{
		// uiManager is only available after Init; setters called earlier just store state.
		if (uiManager == nullptr || fontSource.empty())
			return;

		uiManager->SetFontStyle(fontSource, fontSize, fontStyle);

		auto result = uiManager->LoadText(fontSource, fontSize, text, color, centered);
		textTexture = result.textureID;
		SetWidgetSize(Core::Vector2F(static_cast<float>(result.size.x), static_cast<float>(result.size.y)));
	}

	void TextMesh::SetFontSize(int newFontSize)
	{
		fontSize = newFontSize;

		RebuildTextTexture();
	}

	void TextMesh::SetTextColor(Color newColor)
	{
		color = newColor;

		RebuildTextTexture();
	}

	void TextMesh::SetFontStyle(FontStyle newFontStyle)
	{
		fontStyle = newFontStyle;

		RebuildTextTexture();
	}

	void TextMesh::SetText(std::string newText)
	{
		text = newText;

		RebuildTextTexture();
	}

	void TextMesh::SetIsCentered(bool isCentered)
	{
		centered = isCentered;

		RebuildTextTexture();
	}

	std::string TextMesh::GetText()
	{
		return text;
	}

	RectF TextMesh::GetTextDestRect()
	{
		auto currentPosition = transform->GetWorldPosition();
		auto scaledSize = GetWidgetSize();
		auto anchorVector = GetAnchorFromPreset(anchor);

		return RectF
		{
			.x = currentPosition.x - (scaledSize.x * anchorVector.x),
			.y = currentPosition.y - (scaledSize.y * anchorVector.y),
			.w = scaledSize.x,
			.h = scaledSize.y
		};
	}
}