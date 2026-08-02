#pragma once
#include "Data/Components/Collision/CollisionInfo.h"
#include "Engine/ECS/Component/Collisions/Abstractions/ContactEventProvider.h"
#include <vector>


class FakeContactProvider : public DF2D::Engine::ContactEventProvider
{
	TYPE_INFO(FakeContactProvider, DF2D::Engine::ContactEventProvider);


public:
	std::vector<DF2D::Data::CollisionInfo> enterInfos;

	std::vector<DF2D::Data::CollisionInfo> exitInfos;


	void InvokeCollisionEnter(const DF2D::Data::CollisionInfo& info) override
	{
		enterInfos.push_back(info);
	}

	void InvokeCollisionExit(const DF2D::Data::CollisionInfo& info) override
	{
		exitInfos.push_back(info);
	}
};