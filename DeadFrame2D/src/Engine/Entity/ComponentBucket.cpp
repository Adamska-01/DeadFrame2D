#include "Engine/Components/GameComponent.h"
#include "Engine/EngineEvents/EventDispatcher.h"
#include "Engine/EngineEvents/Events/GameComponentEvents/GameComponentAddedEvent.h"
#include "Engine/Entity/ComponentBucket.h"
#include "Engine/Entity/GameObject.h"
#include "Engine/Entity/Handles/GameObject/ObjectHandle.h"
#include <cassert>


namespace DeadFrame2D::Engine
{
	ComponentBucket::ComponentBucket()
	{
		components.clear();
	}

	ComponentBucket::~ComponentBucket()
	{
		for (auto& cmp : components)
		{
			cmp.alive = false;
			cmp.generation++;
			cmp.instance.reset();
		}

		components.clear();
	}


	GameComponent* ComponentBucket::GetAt(uint32_t index) const
	{
		assert(index < components.size());

		return components[index].instance.get();
	}

	uint32_t ComponentBucket::FindFreeSlot()
	{
		for (size_t i = 0; i < components.size(); ++i)
		{
			if (!components[i].alive)
				return static_cast<uint32_t>(i);
		}

		components.emplace_back();

		return static_cast<uint32_t>(components.size() - 1);
	}

	void ComponentBucket::LinkComponentToOwner(const ObjectHandle<GameObject>& owner, GameComponent* toInitialize)
	{
		toInitialize->SetGameObject(owner);
		toInitialize->RegisterAllHandlers(owner);
	}

	bool ComponentBucket::IsValid(uint32_t index, uint32_t generation) const
	{
		if (index >= components.size())
			return false;

		const auto& entry = components[index];

		return entry.alive && entry.generation == generation;
	}

	void ComponentBucket::SendGameComponentAddedEvent(ComponentHandle<GameComponent> newComponent) const
	{
		EventDispatcher::SendEvent(std::make_shared<GameComponentAddedEvent>(newComponent));
	}
}