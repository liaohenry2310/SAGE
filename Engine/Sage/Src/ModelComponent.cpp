#include "Precompiled.h"
#include "ModelComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace Sage;
using namespace Sage::Graphics;

MEMORY_POOL_DEFINE(ModelComponent, 1000);

void Sage::ModelComponent::Initialize()
{
	const char* modelFileName = mFileName.c_str();
	ModelIO::LoadModel(modelFileName, mModel);
	ModelIO::LoadMaterial(modelFileName, mModel);
	ModelIO::LoadSkeleton(modelFileName, mModel);

	GetOwner().GetWorld().GetService<RenderService>()->Register(this);
}

void Sage::ModelComponent::Terminate()
{
	GetOwner().GetWorld().GetService<RenderService>()->Unregister(this);
}
