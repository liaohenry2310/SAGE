#include "Precompiled.h"
#include "ParticleEmitterComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "RenderService.h"

using namespace Sage;
using namespace Sage::Graphics;

MEMORY_POOL_DEFINE(ParticleEmitterComponent, 1000);

void ParticleEmitterComponent::Initialize()
{
	GetOwner().GetWorld().GetService<RenderService>()->Register(this);
	mParticleEmitter.Initialize();
}

void ParticleEmitterComponent::Terminate()
{
	GetOwner().GetWorld().GetService<RenderService>()->Unregister(this);
	mParticleEmitter.Terminate();
}

void ParticleEmitterComponent::Update(float deltaTime)
{
	auto transform = GetOwner().GetComponent<TransformComponent>();
	mParticleEmitter.SetPosition(transform->position + mPosOffset + Math::GetLook(transform->GetMatrix4()));
	mParticleEmitter.SetDirection(Math::GetLook(transform->GetMatrix4()), 0.0f);
	mParticleEmitter.Update(deltaTime);
}

void ParticleEmitterComponent::DebugUI()
{
}
