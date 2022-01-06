#include "Precompiled.h"
#include "ColliderComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

using namespace Sage;

MEMORY_POOL_DEFINE(ColliderComponent, 1000);

void Sage::ColliderComponent::Initialize()
{
	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void Sage::ColliderComponent::DebugUI()
{
	const auto& aabb = GetAABB();
	Graphics::SimpleDraw::AddAABB(aabb.center, aabb.extend, Sage::Graphics::Colors::Green);
}

Math::AABB Sage::ColliderComponent::GetAABB()
{
	Math::AABB aabb;
	aabb.center = mTransformComponent->position + mCenter;
	aabb.extend = mExtend;
	return aabb;
}
