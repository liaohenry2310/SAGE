#include "Precompiled.h"
#include "TransformComponent.h"

using namespace Sage;

MEMORY_POOL_DEFINE(TransformComponent, 1000);

void Sage::TransformComponent::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}
