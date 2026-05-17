#pragma once
#include "Core/Math/Vector2.h"
#include "DF2D_API.h"
#include "Engine/ECS/Entity/Component/Handle/ComponentHandle.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include <string_view>


namespace DF2D::Engine
{
	class AudioSource;


	class DF2D_API AudioClipBlueprint : public GameObject
	{
	private:
		ComponentHandle<AudioSource> audioSource;

		std::string_view audioSourcePath;

		Core::Vector2F position;

		float volume;

		bool isMusic;

		bool loop;


	public:
		AudioClipBlueprint(
			const std::string_view& audioSourcePath,
			Core::Vector2F position = Core::Vector2F::Zero,
			float volume = 1.0f,
			bool isMusic = false,
			bool loop = false);


		virtual void ConstructGameObject() override;


		ComponentHandle<AudioSource> GetAudioSource();
	};
}