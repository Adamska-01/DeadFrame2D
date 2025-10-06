#pragma once
#include "DF2D_API.h"
#include "Core/Math/Vector2.h"
#include "Engine/Entity/GameObject.h"
#include <string_view>


namespace DeadFrame2D::Engine
{
	class AudioSource;


	class DF2D_API AudioClipBlueprint : public GameObject
	{
	private:
		AudioSource* audioSource;

		std::string_view audioSourcePath;

		DeadFrame2D::Core::Vector2F position;

		float volume;

		bool isMusic;

		bool loop;


	public:
		AudioClipBlueprint(
			const std::string_view& audioSourcePath,
			DeadFrame2D::Core::Vector2F position = DeadFrame2D::Core::Vector2F::Zero,
			float volume = 1.0f,
			bool isMusic = false,
			bool loop = false);


		virtual void ConstructGameObject() override;


		AudioSource* GetAudioSource();
	};
}