#pragma once
#include "Constants/AssetPaths.h"
#include "Scenes/Abstractions/BaseGameScene.h"
#include <Blueprints/UI/ButtonBlueprint.h>
#include <Components/UI/TextMesh.h>
#include <Coroutines/CoroutineScheduler.h>
#include <Models/Blueprints/UI/ButtonBlueprintModel.h>
#include <Models/Components/UI/ButtonComponentModel.h>
#include <Models/Components/UI/TextMeshComponentModel.h>
#include <Prefabs/AudioClipObject.h>


std::weak_ptr<ButtonBlueprint> BaseGameScene::CreateButton(const std::string& text, const std::function<void()>& onPressedHandler, const std::function<void()>& onEnterHandler)
{
	ButtonBlueprintModel buttonConfiguration =
	{
		ButtonComponentModel
		{
			.onPressedHandler = onPressedHandler,
			.onEnterHandler = onEnterHandler,
			.idleButtonSource = AssetPaths::Sprites::BUTTON_IDLE_IMAGE_PATH,
			.hoveredButtonSource = AssetPaths::Sprites::BUTTON_PRESSED_IMAGE_PATH,
			.pressedButtonSource = AssetPaths::Sprites::BUTTON_PRESSED_IMAGE_PATH,
			.buttonSize = Vector2F{ 275.0f, 80.0f }
		},
		TextMeshComponentModel
		{
			.fontSource = AssetPaths::Fonts::THE_BLAST_FONT_PATH,
			.text = text,
			.textColor = SDL_Color(255, 132, 31),
			.fontSize = 100,
			.textObjectInitialScale = Vector2F(0.25f, 0.25f),
			.isCentered = true
		}
	};

	return GameObject::Instantiate<ButtonBlueprint>(buttonConfiguration);
}

std::weak_ptr<GameObject> BaseGameScene::CreateText(const std::string& text)
{
	auto textMeshObject = GameObject::Instantiate<GameObject>();

	textMeshObject.lock()->AddComponent<TextMesh>(TextMeshComponentModel
		{
			.fontSource = AssetPaths::Fonts::THE_BLAST_FONT_PATH,
			.text = text,
			.textColor = SDL_Color(255, 132, 31),
			.fontSize = 100,
			.textObjectInitialScale = Vector2F(0.25f, 0.25f),
			.isCentered = true
		});

	return textMeshObject;
}

std::function<void()> BaseGameScene::MakeAudioPlayAndDestroyCallback(const std::string_view& audioPath, const Vector2F& position, float volume, bool isMusic, bool loop, float destroyDelaySeconds)
{
	return [=]()
		{
			auto soundSourceObj = GameObject::Instantiate<AudioClipObject>(
				audioPath,
				position,
				volume,
				isMusic,
				loop);

			CoroutineScheduler::StartCoroutine(soundSourceObj.lock()->Destroy(destroyDelaySeconds));
		};
}