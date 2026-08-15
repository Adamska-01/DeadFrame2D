#pragma once
#include "Data/Services/ServiceContext.h"
#include "Data/Systems/CoreContext.h"
#include "Engine/ECS/Entity/Object/Core/GameObject.h"
#include "Engine/ECS/Entity/Object/Core/GameObjectConstructionContext.h"
#include "Engine/ECS/Entity/Object/Handle/ObjectHandle.h"
#include "Engine/ECS/System/Scene/Abstractions/ISceneHandleProvider.h"
#include <memory>
#include <vector>


// Minimal stand-in for Scene: owns GameObjects behind ISceneHandleProvider so
// ObjectHandle<T> resolves without a real Scene/SceneManager. Mirrors
// Scene::Instantiate's allocate-then-placement-construct sequence so a GameObject's
// own constructor can resolve its handle (see GameObjectConstructionContext).
class FakeSceneHandleProvider
	: public DF2D::Engine::ISceneHandleProvider,
	public std::enable_shared_from_this<FakeSceneHandleProvider>
{
private:
	struct Entry
	{
		std::unique_ptr<DF2D::Engine::GameObject> object;

		uint32_t generation = 0;

		bool alive = false;
	};

	std::vector<Entry> entries;


public:
	template<typename T, typename... Args>
	DF2D::Engine::ObjectHandle<T> Create(Args&&... args);

	void Destroy(uint32_t index);


	DF2D::Engine::GameObject* GetAt(uint32_t index) const override;

	bool IsValid(uint32_t index, uint32_t generation) const override;
};


template<typename T, typename... Args>
inline DF2D::Engine::ObjectHandle<T> FakeSceneHandleProvider::Create(Args&&... args)
{
	entries.emplace_back();

	auto index = static_cast<uint32_t>(entries.size() - 1);
	auto& entry = entries[index];
	entry.alive = true;

	// Reserve raw storage and register it before construction, so T's own constructor
	// can resolve its own handle (mirrors Scene::Instantiate).
	void* storage = ::operator new(sizeof(T));
	entry.object = std::unique_ptr<DF2D::Engine::GameObject>(static_cast<DF2D::Engine::GameObject*>(static_cast<T*>(storage)));

	auto handle = DF2D::Engine::ObjectHandle<T>(weak_from_this(), index, entry.generation);

	DF2D::Engine::GameObjectConstructionContext constructionContext(handle, DF2D::Data::CoreContext{}, DF2D::Data::ServiceContext{});

	try
	{
		new (storage) T(std::forward<Args>(args)...);
	}
	catch (...)
	{
		entry.object.release();
		::operator delete(storage);
		entry.alive = false;

		throw;
	}

	return handle;
}

inline DF2D::Engine::GameObject* FakeSceneHandleProvider::GetAt(uint32_t index) const
{
	if (index >= entries.size())
		return nullptr;

	return entries[index].alive ? entries[index].object.get() : nullptr;
}

inline bool FakeSceneHandleProvider::IsValid(uint32_t index, uint32_t generation) const
{
	if (index >= entries.size())
		return false;

	const auto& entry = entries[index];

	return entry.alive && entry.generation == generation;
}

inline void FakeSceneHandleProvider::Destroy(uint32_t index)
{
	if (index >= entries.size())
		return;

	entries[index].alive = false;
	entries[index].generation++;
}