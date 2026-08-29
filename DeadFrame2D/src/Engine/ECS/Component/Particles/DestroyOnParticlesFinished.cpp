#include "Engine/ECS/Component/Particles/DestroyOnParticlesFinished.h"
#include "Engine/ECS/Component/Particles/ParticleEmitter.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Utilities/Debugging/Guards.h"


namespace DF2D::Engine
{
	using namespace DF2D::Utilities;


	DestroyOnParticlesFinished::DestroyOnParticlesFinished()
	{
	}


	void DestroyOnParticlesFinished::Init()
	{
		emitter = Guard::AgainstNullAssignment(GetGameObject()->GetComponent<ParticleEmitter>(), NAME_OF(emitter));
	}

	void DestroyOnParticlesFinished::Update(float deltaTime)
	{
		if (!emitter->IsFinished())
			return;

		GetGameObject()->Destroy();
	}
}